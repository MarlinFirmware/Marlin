cmake_minimum_required(VERSION 2.8)
#====================================================================#
# Usage under Linux:                                                 #
#                                                                    #
#    From buildroot/share/cmake folder:                              #
#        mkdir -p build && cd build                                  #
#        cmake ..                                                    #
#        make                                                        #
#                                                                    #
# Usage under Windows:                                               #
#                                                                    #
#    From buildroot/share/cmake folder:                              #
#        mkdir build && cd build                                     #
#        cmake -G"Unix Makefiles" ..                                 #
#        make                                                        #
#====================================================================#
 
#====================================================================#
# Download marlin-cmake scriptfiles if not already installed         #
# and add the path to the module path                                #
#====================================================================#

if(NOT EXISTS ${CMAKE_BINARY_DIR}/marlin-cmake)

    file(DOWNLOAD https://raw.githubusercontent.com/tohara/marlin-cmake/v1.0.0/modules/Arduino_SDK.cmake
                  ${CMAKE_BINARY_DIR}/marlin-cmake/modules/Arduino_SDK.cmake SHOW_PROGRESS)
                  
    file(DOWNLOAD https://raw.githubusercontent.com/tohara/marlin-cmake/v1.0.0/modules/marlin_cmake_functions.cmake
                  ${CMAKE_BINARY_DIR}/marlin-cmake/modules/marlin_cmake_functions.cmake SHOW_PROGRESS)             
    
    file(DOWNLOAD https://raw.githubusercontent.com/tohara/marlin-cmake/v1.0.0/Platform/Arduino.cmake
                  ${CMAKE_BINARY_DIR}/marlin-cmake/Platform/Arduino.cmake SHOW_PROGRESS)
    
    file(DOWNLOAD https://raw.githubusercontent.com/tohara/marlin-cmake/v1.0.0/settings/marlin_boards.txt
                  ${CMAKE_BINARY_DIR}/marlin-cmake/settings/marlin_boards.txt SHOW_PROGRESS)
                  
    file(DOWNLOAD https://raw.githubusercontent.com/tohara/marlin-cmake/v1.0.0/toolchain/ArduinoToolchain.cmake
                  ${CMAKE_BINARY_DIR}/marlin-cmake/toolchain/ArduinoToolchain.cmake SHOW_PROGRESS)
              
    if(WIN32)
        file(DOWNLOAD https://raw.githubusercontent.com/tohara/marlin-cmake/v1.0.0/resources/make.exe
                      ${CMAKE_BINARY_DIR}/make.exe SHOW_PROGRESS)
    endif(WIN32)

endif()

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR}/marlin-cmake/modules)

#====================================================================#
#  Custom path to Arduino SDK can be set here.                       #
#  It can also be set from command line. eg.:                        #
#  cmake .. -DARDUINO_SDK_PATH="/path/to/arduino-1.x.x"              #
#====================================================================#
#set(ARDUINO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/arduino-1.6.8)
#set(ARDUINO_SDK_PATH /home/tom/git/BigBox-Dual-Marlin/ArduinoAddons/Arduino_1.6.x)
#set(ARDUINO_SDK_PATH  /home/tom/test/arduino-1.6.11)
#====================================================================#
#  Set included cmake files                                          #
#====================================================================#
include(Arduino_SDK) # Find the intallpath of Arduino SDK
include(marlin_cmake_functions)

#====================================================================#
#  Set toolchain file for arduino                                    #
#====================================================================#
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/marlin-cmake/toolchain/ArduinoToolchain.cmake) # Arduino Toolchain

#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(Marlin C CXX)

#====================================================================#
#  Register non standard hardware                                    #
#====================================================================#
#register_hardware_platform(/home/tom/test/Sanguino)

#====================================================================#
#  Print any info                                                    #
#  print_board_list()                                                #
#  print_programmer_list()                                           #
#  print_board_settings(mega)                                        #
#====================================================================#
print_board_list()
print_programmer_list()

#====================================================================#
#  add the project directory into build                              #
#====================================================================#
add_subdirectory(../../../Marlin ./Marlin)
