precompiled libraries for the HC32F46x core provided by voxlab in `hdsc.a`. 
unpacking the library allows me to exclude specific files (like the botched `dtostrf` implementation) when linking.

all object files in this library are included in `LIB_OBJ_FILES` in HC32.mk and linked with the rest of the program.

