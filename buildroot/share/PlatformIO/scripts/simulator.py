#
# simulator.py
# PlatformIO pre: script for simulator builds
#
# On macOS the system 'gcc' is Clang, which can't build Marlin. This script finds a real GCC
# (MacPorts or Homebrew) and uses it for this build only, with no global PATH or symlink changes.
# To choose a specific compiler, set CXX (and optionally CC) for the build:
#
#   CXX=g++-15 pio run -e simulator_macos_debug
#
# The platform builder detects 'gcc' and 'g++' on the build PATH and clones the environment
# for the project and each library, so the compiler can't simply be set by CC / CXX here.
# Instead the chosen compiler is linked as gcc / g++ in a private bin folder in the build
# folder, put first on the build PATH. This script must run before common-dependencies.py
# so the config preprocessor uses the same compiler.
#

import pioutil
if pioutil.is_pio_build():
    # Get the environment thus far for the build
    env = pioutil.env

    #print(env.Dump())

    #
    # Give the binary a distinctive name
    #

    env['PROGNAME'] = "MarlinSimulator"

    #
    # Check for a valid GCC and available OpenGL on macOS
    #
    emsg = ''
    fatal = 0
    import sys
    if sys.platform == 'darwin':

        import os, os.path, re, shutil, subprocess

        #
        # Find the package-manager prefix (Homebrew or MacPorts) so SDL2, glm,
        # and freetype headers/libs are found regardless of install location.
        #
        prefix = ''
        brew = shutil.which('brew')
        if brew:
            try: prefix = subprocess.check_output([ brew, '--prefix' ], text=True).strip()
            except Exception: prefix = ''
        if not prefix and os.path.exists('/opt/local'):
            prefix = '/opt/local'   # MacPorts

        #
        # Find a real GCC as a dict of full paths { 'gcc', 'g++', 'cpp' (optional) }, or None
        #
        GCC_MIN_VERSION = 11

        def is_gcc(path):
            # Apple's gcc / g++ are Clang
            try: out = subprocess.check_output([ path, '--version' ], text=True, stderr=subprocess.STDOUT)
            except Exception: return False
            return 'clang' not in out.lower()

        def gcc_tools(cxx, cc=None):
            # Tool set for a g++, with its gcc / cpp siblings (same name, same suffix)
            cxx = shutil.which(cxx) or cxx
            if not (os.path.isfile(cxx) and is_gcc(cxx)): return None
            folder, name = os.path.split(cxx)
            cc = (shutil.which(cc) or cc) if cc else os.path.join(folder, name.replace('g++', 'gcc', 1))
            if not os.path.isfile(cc): return None
            tools = { 'gcc': cc, 'g++': cxx }
            cpp = os.path.join(folder, name.replace('g++', 'cpp', 1))
            if os.path.isfile(cpp): tools['cpp'] = cpp
            return tools

        def newest_gcc(folder, pattern):
            # The newest versioned g++ (e.g. g++-mp-14, g++-15) in a folder, at least GCC_MIN_VERSION
            if not os.path.isdir(folder): return None
            found = []
            for name in os.listdir(folder):
                m = re.fullmatch(pattern, name)
                if m and int(m.group(1)) >= GCC_MIN_VERSION: found.append((int(m.group(1)), name))
            for _, name in sorted(found, reverse=True):
                tools = gcc_tools(os.path.join(folder, name))
                if tools: return tools
            return None

        def find_gcc():
            # 1. CXX (and CC) from the environment
            if os.environ.get('CXX'):
                tools = gcc_tools(os.environ['CXX'], os.environ.get('CC'))
                if tools: return tools
            # 2. 'g++' on the PATH, if it's GCC (e.g. set up by buildroot/bin/mac_gcc)
            if shutil.which('g++'):
                tools = gcc_tools('g++', 'gcc')
                if tools: return tools
            # 3. The newest GCC from MacPorts, then Homebrew
            port = shutil.which('port')
            if port:
                tools = newest_gcc(os.path.dirname(os.path.realpath(port)), r'g\+\+-mp-(\d+)')
                if tools: return tools
            if prefix:
                tools = newest_gcc(os.path.join(prefix, 'bin'), r'g\+\+-(\d+)')
                if tools: return tools
            return None

        gcc = find_gcc()
        if not gcc:

            emsg = "\u001b[31mCan't build Marlin Native on macOS with Clang, and no GCC %d or newer was found." % GCC_MIN_VERSION
            emsg += "\n\u001b[31mSee 'native.ini' for instructions to install GCC with MacPorts or Homebrew."
            fatal = 1

        else:

            #
            # Link the chosen compiler as gcc / g++ / cpp in a private bin folder and put it
            # first on the build PATH (for the compiler and linker) and on the PATH of this
            # process (for the config preprocessor). The user's shell is not affected.
            #
            gcc_bin = os.path.join(env['PROJECT_BUILD_DIR'], env['PIOENV'], 'gcc-bin')
            os.makedirs(gcc_bin, exist_ok=True)
            for name in ('gcc', 'g++', 'cpp'):
                link = os.path.join(gcc_bin, name)
                if os.path.lexists(link): os.remove(link)
                if name in gcc: os.symlink(gcc[name], link)

            env['ENV']['PATH'] = gcc_bin + os.pathsep + env['ENV']['PATH']
            os.environ['PATH'] = gcc_bin + os.pathsep + os.environ['PATH']
            print(f"Simulator compiler: {gcc['g++']}")

            #
            # Silence half of the ranlib warnings. (No equivalent for 'ARFLAGS')
            #
            env['RANLIBFLAGS'] += [ "-no_warning_for_no_symbols" ]

            if prefix:
                env['BUILD_FLAGS'] += [ '-I' + prefix + '/include',
                                        '-I' + prefix + '/include/freetype2',
                                        '-I' + prefix + '/include/SDL2',
                                        '-L' + prefix + '/lib' ]

            # Default paths for Xcode and a lucky GL/gl.h dropped by Mesa
            xcode_path = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"
            mesa_path = "/opt/local/include/GL/gl.h"

            # Command Line Tools SDK frameworks (no full Xcode.app required)
            clt_fw = ''
            try:
                sdk = subprocess.check_output([ 'xcrun', '--show-sdk-path' ], text=True).strip()
                if sdk: clt_fw = sdk + "/System/Library/Frameworks"
            except Exception:
                pass

            if os.path.exists(xcode_path):

                env['BUILD_FLAGS'] += [ "-F" + xcode_path ]
                emsg = "\u001b[33mUsing OpenGL framework headers from Xcode.app"

            elif clt_fw and os.path.exists(clt_fw + "/OpenGL.framework/Headers/gl.h"):

                env['BUILD_FLAGS'] += [ "-F" + clt_fw ]
                emsg = "\u001b[33mUsing OpenGL framework headers from the Command Line Tools SDK"

            elif os.path.exists(mesa_path):

                env['BUILD_FLAGS'] += [ '-D__MESA__' ]
                emsg = f"\u001b[33mUsing OpenGL header from {mesa_path}"

            else:

                emsg = "\u001b[31mNo OpenGL headers found. Install the Command Line Tools (xcode-select --install) or Mesa."
                fatal = 1

    # Print error message, if any
    if emsg: print(f"\n\n{emsg}\n\n")

    # Break out of the PIO build immediately
    if fatal: sys.exit(1)
