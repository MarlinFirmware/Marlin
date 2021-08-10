# Automatic configuration saving in built firmware

Starting with version 2.0.9.2, Marlin automatically extracts and saves the configuration used to generate the firmware into the firmware itself.
This is can be avoided by defining `CONFIG_SKIP_EMBED_AND_SAVE_TO_SD` in `Configuration_adv.h` file.

## How is it made
A first pass is applied on the configuration files to extract all active definitions in them. Then a parser is used to compute a key/value pair of the defined macro and its value.
No expansion is done to the macros here.

Then, a file called `marlin_config.json` is serialized from this file/key/value tree, including specific build information (like the git revision, the building date and some versioning information.

Once the JSON file is written, it's compressed to `.pio/build/mc.zip` file and then converted to a C array in a generated `.pio/build/mc.cpp` file, that's included in the build list.

## Retrieving the configuration from a binary firmware
To retrieve the configuration from a binary firmware, you'll have to use a non-write protected SD card in the printer running the firmware.
Once this is done, you'll then issue a `M503 C1` to the printer. It'll write this `mc.zip` file onto the SD card.

Retrieve the SD card and copy the file back to your computer, ideally in the same folder as the expected Marlin's repository.
Then run the following command:
```
$ git checkout -f
$ unzip mc.zip 
$ python buildroot/share/PlatformIO/scripts/create_config_from_dict.py
```

This will update the configuration file in the repository to match what was used at build time. It'll also dump the Git's reference used to build the code (that might be accessible if the 
firmware was built from official repository, or, as a fallback, it'll also dump the `STRING_DISTRIBUTION_DATE` which is unlikely to be modified in a fork). 


 
 