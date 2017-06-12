These configuration files are unofficial (community supported) configs for
the MakerFarm Pegasus printers.  I've only tested with a Pegasus 10", but I
presume that to use on a Pegasus 8" or 12" the only change needed is to change
the following defines:

#define X_MAX_POS 250 // kevinh makerfarm 10
#define Y_MAX_POS 250
#define Z_MAX_POS 250

These configuration files are based off the 1.1.1 template files, but with 
changes from the (old) MakerFarm 1.0.2 tree.
