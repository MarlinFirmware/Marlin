cmake_minimum_required(VERSION 2.8)
#====================================================================#
# Usage under Linux:                                                 #
#                                                                    #
#    From Marlin/buildroot/share/cmake folder:                       #
#        mkdir -p build && cd build                                  #
#        cmake ..                                                    #
#        make                                                        #
#                                                                    #
# Usage under Windows:                                               #
#                                                                    #
#    From Marlin/buildroot/share/cmake folder:                       #
#        mkdir build && cd build                                     #
#        cmake -G"Unix Makefiles" ..                                 #
#        make                                                        #
#====================================================================#

#====================================================================#
# Download marlin-cmake scriptfiles if not already installed         #
# and add the path to the module path                                #
#====================================================================#

set(SCRIPT_BRANCH 1.0.2) #Set to wanted marlin-cmake release tag or branch

if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake)

	file(DOWNLOAD https://github.com/tohara/marlin-cmake/archive/${SCRIPT_BRANCH}.tar.gz
	${CMAKE_CURRENT_LIST_DIR}/marlin-cmake-src.tar.gz SHOW_PROGRESS)

	execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xvf ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake-src.tar.gz WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

	file(RENAME ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake-${SCRIPT_BRANCH} ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake)
	file(REMOVE ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake-src.tar.gz)


endif()

if(WIN32 AND NOT EXISTS ${CMAKE_BINARY_DIR}/make.exe)
	file(COPY ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake/resources/make.exe DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake/modules)

#====================================================================#
#  Custom path to Arduino SDK can be set here.                       #
#  It can also be set from command line. eg.:                        #
#  cmake .. -DARDUINO_SDK_PATH="/path/to/arduino-1.x.x"              #
#====================================================================#
#set(ARDUINO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/arduino-1.6.8)
#set(ARDUINO_SDK_PATH /Applications/Arduino.app/Contents/Java)
#set(ARDUINO_SDK_PATH $HOME/ArduinoAddons/Arduino_1.6.x)
#====================================================================#
#  Set included cmake files                                          #
#====================================================================#
include(Arduino_SDK) # Find the intallpath of Arduino SDK
include(marlin_cmake_functions)

#====================================================================#
#  Set toolchain file for arduino                                    #
#====================================================================#
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/marlin-cmake/toolchain/ArduinoToolchain.cmake) # Arduino Toolchain

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
#  Get motherboard settings from Configuration.h                     #
#  setup_motherboard(TARGET Marlin_src_folder)                       #
#  Returns ${TARGET}_BOARD and ${TARGET}_CPU                         #
#                                                                    #
#  To set it manually:                                               #
#    set(${PROJECT_NAME}_BOARD mega)                                 #
#    set(${PROJECT_NAME}_CPU atmega2560)                             #
#====================================================================#
setup_motherboard(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/../../../Marlin)

#====================================================================#
#  Setup all source files                                            #
#  Include Marlin.ino to compile libs not included in *.cpp files    #
#====================================================================#

file(GLOB_RECURSE SOURCES "../../../Marlin/*.cpp")
set(${PROJECT_NAME}_SRCS "${SOURCES};../../../Marlin/Marlin.ino")

#====================================================================#
#  Define the port for uploading code to the Arduino                 #
#  Can be set from commandline with:                                 #
#    cmake .. -DUPLOAD_PORT=/dev/ttyACM0                             #
#====================================================================#
if(UPLOAD_PORT)
	set(${PROJECT_NAME}_PORT ${UPLOAD_PORT})
else()
	set(${PROJECT_NAME}_PORT /dev/ttyACM0)
endif()

#====================================================================#
#  Register arduino libraries not included in SDK                    #
#====================================================================#
#link_directories(/home/tom/test/ArduinoAddons) #U8glib
#set(${PROJECT_NAME}_ARDLIBS U8glib)
#set(U8glib_RECURSE True)

#====================================================================#
# Command to generate code arduino firmware (.hex file)              #
#====================================================================#
generate_arduino_firmware(${PROJECT_NAME})
