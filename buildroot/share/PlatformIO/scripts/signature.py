#
# signature.py
#
import schema

import subprocess,re,json,hashlib
from datetime import datetime
from pathlib import Path

#
# Return all macro names in a header as an array, so we can take
# the intersection with the preprocessor output, giving a decent
# reflection of all enabled options that (probably) came from the
# configuration files. We end up with the actual configured state,
# better than what the config files say. You can then use the
# resulting config.ini to produce more exact configuration files.
#
def extract_defines(filepath):
    f = open(filepath, encoding="utf8").read().split("\n")
    a = []
    for line in f:
        sline = line.strip()
        if sline[:7] == "#define":
            # Extract the key here (we don't care about the value)
            kv = sline[8:].strip().split()
            a.append(kv[0])
    return a

# Compute the SHA256 hash of a file
def get_file_sha256sum(filepath):
    sha256_hash = hashlib.sha256()
    with open(filepath,"rb") as f:
        # Read and update hash string value in blocks of 4K
        for byte_block in iter(lambda: f.read(4096),b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

#
# Compress a JSON file into a zip file
#
import zipfile
def compress_file(filepath, outpath):
    with zipfile.ZipFile(outpath, 'w', compression=zipfile.ZIP_BZIP2, compresslevel=9) as zipf:
        zipf.write(filepath, compress_type=zipfile.ZIP_BZIP2, compresslevel=9)

#
# Compute the build signature. The idea is to extract all defines in the configuration headers
# to build a unique reversible signature from this build so it can be included in the binary
# We can reverse the signature to get a 1:1 equivalent configuration file
#
def compute_build_signature(env):
    if 'BUILD_SIGNATURE' in env:
        return

    # Definitions from these files will be kept
    files_to_keep = [ 'Marlin/Configuration.h', 'Marlin/Configuration_adv.h' ]

    build_path = Path(env['PROJECT_BUILD_DIR'], env['PIOENV'])

    # Check if we can skip processing
    hashes = ''
    for header in files_to_keep:
        hashes += get_file_sha256sum(header)[0:10]

    marlin_json = build_path / 'marlin_config.json'
    marlin_zip = build_path / 'mc.zip'

    # Read existing config file
    try:
        with marlin_json.open() as infile:
            conf = json.load(infile)
            if conf['__INITIAL_HASH'] == hashes:
                # Same configuration, skip recomputing the building signature
                compress_file(marlin_json, marlin_zip)
                return
    except:
        pass

    # Get enabled config options based on preprocessor
    from preprocessor import run_preprocessor
    complete_cfg = run_preprocessor(env)

    # Dumb #define extraction from the configuration files
    conf_defines = {}
    all_defines = []
    for header in files_to_keep:
        defines = extract_defines(header)
        # To filter only the define we want
        all_defines += defines
        # To remember from which file it cames from
        conf_defines[header.split('/')[-1]] = defines

    r = re.compile(r"\(+(\s*-*\s*_.*)\)+")

    # First step is to collect all valid macros
    defines = {}
    for line in complete_cfg:

        # Split the define from the value
        key_val = line[8:].strip().decode().split(' ')
        key, value = key_val[0], ' '.join(key_val[1:])

        # Ignore values starting with two underscore, since it's low level
        if len(key) > 2 and key[0:2] == "__" :
            continue
        # Ignore values containing a parenthesis (likely a function macro)
        if '(' in key and ')' in key:
            continue

        # Then filter dumb values
        if r.match(value):
            continue

        defines[key] = value if len(value) else ""

    #
    # Continue to gather data for CONFIGURATION_EMBEDDING or CONFIG_EXPORT
    #
    if not ('CONFIGURATION_EMBEDDING' in defines or 'CONFIG_EXPORT' in defines):
        return

    # Second step is to filter useless macro
    resolved_defines = {}
    for key in defines:
        # Remove all boards now
        if key.startswith("BOARD_") and key != "BOARD_INFO_NAME":
            continue
        # Remove all keys ending by "_NAME" as it does not make a difference to the configuration
        if key.endswith("_NAME") and key != "CUSTOM_MACHINE_NAME":
            continue
        # Remove all keys ending by "_T_DECLARED" as it's a copy of extraneous system stuff
        if key.endswith("_T_DECLARED"):
            continue
        # Remove keys that are not in the #define list in the Configuration list
        if key not in all_defines + [ 'DETAILED_BUILD_VERSION', 'STRING_DISTRIBUTION_DATE' ]:
            continue

        # Don't be that smart guy here
        resolved_defines[key] = defines[key]

    # Generate a build signature now
    # We are making an object that's a bit more complex than a basic dictionary here
    data = {}
    data['__INITIAL_HASH'] = hashes
    # First create a key for each header here
    for header in conf_defines:
        data[header] = {}

    # Then populate the object where each key is going to (that's a O(N^2) algorithm here...)
    for key in resolved_defines:
        for header in conf_defines:
            if key in conf_defines[header]:
                data[header][key] = resolved_defines[key]

    # Every python needs this toy
    def tryint(key):
        try:
            return int(defines[key])
        except:
            return 0

    config_dump = tryint('CONFIG_EXPORT')

    #
    # Produce an INI file if CONFIG_EXPORT == 2
    #
    if config_dump == 2:
        print("Generating config.ini ...")
        config_ini = build_path / 'config.ini'
        with config_ini.open('w') as outfile:
            ignore = ('CONFIGURATION_H_VERSION', 'CONFIGURATION_ADV_H_VERSION', 'CONFIG_EXPORT')
            filegrp = { 'Configuration.h':'config:basic', 'Configuration_adv.h':'config:advanced' }
            vers = defines["CONFIGURATION_H_VERSION"]
            dt_string = datetime.now().strftime("%Y-%m-%d at %H:%M:%S")
            ini_fmt = '{0:40}{1}\n'
            outfile.write(
                '#\n'
                + '# Marlin Firmware\n'
                + '# config.ini - Options to apply before the build\n'
                + '#\n'
                + f'# Generated by Marlin build on {dt_string}\n'
                + '#\n'
                + '\n'
                + '[config:base]\n'
                + ini_fmt.format('ini_use_config', ' = all')
                + ini_fmt.format('ini_config_vers', f' = {vers}')
            )
            # Loop through the data array of arrays
            for header in data:
                if header.startswith('__'):
                    continue
                outfile.write('\n[' + filegrp[header] + ']\n')
                for key in sorted(data[header]):
                    if key not in ignore:
                        val = 'on' if data[header][key] == '' else data[header][key]
                        outfile.write(ini_fmt.format(key.lower(), ' = ' + val))

    #
    # Produce a schema.json file if CONFIG_EXPORT == 3
    #
    if config_dump >= 3:
        try:
            conf_schema = schema.extract()
        except Exception as exc:
            print("Error: " + str(exc))
            conf_schema = None

        if conf_schema:
            #
            # Produce a schema.json file if CONFIG_EXPORT == 3
            #
            if config_dump in (3, 13):
                print("Generating schema.json ...")
                schema.dump_json(conf_schema, build_path / 'schema.json')
                if config_dump == 13:
                    schema.group_options(conf_schema)
                    schema.dump_json(conf_schema, build_path / 'schema_grouped.json')

            #
            # Produce a schema.yml file if CONFIG_EXPORT == 4
            #
            elif config_dump == 4:
                print("Generating schema.yml ...")
                try:
                    import yaml
                except ImportError:
                    env.Execute(env.VerboseAction(
                        '$PYTHONEXE -m pip install "pyyaml"',
                        "Installing YAML for schema.yml export",
                    ))
                    import yaml
                schema.dump_yaml(conf_schema, build_path / 'schema.yml')

    # Append the source code version and date
    data['VERSION'] = {}
    data['VERSION']['DETAILED_BUILD_VERSION'] = resolved_defines['DETAILED_BUILD_VERSION']
    data['VERSION']['STRING_DISTRIBUTION_DATE'] = resolved_defines['STRING_DISTRIBUTION_DATE']
    try:
        curver = subprocess.check_output(["git", "describe", "--match=NeVeRmAtCh", "--always"]).strip()
        data['VERSION']['GIT_REF'] = curver.decode()
    except:
        pass

    #
    # Produce a JSON file for CONFIGURATION_EMBEDDING or CONFIG_EXPORT == 1
    #
    if config_dump == 1 or 'CONFIGURATION_EMBEDDING' in defines:
        with marlin_json.open('w') as outfile:
            json.dump(data, outfile, separators=(',', ':'))

    #
    # The rest only applies to CONFIGURATION_EMBEDDING
    #
    if not 'CONFIGURATION_EMBEDDING' in defines:
        return

    # Compress the JSON file as much as we can
    compress_file(marlin_json, marlin_zip)

    # Generate a C source file for storing this array
    with open('Marlin/src/mczip.h','wb') as result_file:
        result_file.write(
              b'#ifndef NO_CONFIGURATION_EMBEDDING_WARNING\n'
            + b'  #warning "Generated file \'mc.zip\' is embedded (Define NO_CONFIGURATION_EMBEDDING_WARNING to suppress this warning.)"\n'
            + b'#endif\n'
            + b'const unsigned char mc_zip[] PROGMEM = {\n '
        )
        count = 0
        for b in (build_path / 'mc.zip').open('rb').read():
            result_file.write(b' 0x%02X,' % b)
            count += 1
            if count % 16 == 0:
                result_file.write(b'\n ')
        if count % 16:
            result_file.write(b'\n')
        result_file.write(b'};\n')
