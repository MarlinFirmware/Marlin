## 
## Makefile for HC32F46x based printers
## - build: `make -f H32.mk all`
## - clean: `make -f H32.mk clean`
##
## requires gcc-arm-none-eabi toolchain to be installed. see .toolchain/ for details 
##

# run builds in parallel
# good for speed, but harder to debug...
MAKEFLAGS += -j8

## Util Functions ##
# recursive wildcard, see https://stackoverflow.com/a/12959764
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# findstring, but with multiple search strings, based on https://stackoverflow.com/a/59532390
EMPTY = 
SPACE = $(EMPTY) $(EMPTY)
findany = $(subst $(SPACE),,$(foreach W,$1,$(findstring $W,$2)))

# list does not contain any entry, based on https://stackoverflow.com/a/12324443
not-containing = $(foreach v,$2,$(if $(call findany,$1,$v),,$v))

## Target Device Configuration ##
TARGET_DEVICE_LD = hc32f460xCxx_bl

## File path configuration ##
BUILD_DIR_NAME = build
C_FILES = $(call rwildcard,,*.c)
CPP_FILES = $(call rwildcard,,*.cpp)
ASM_FILES = $(call rwildcard,,*.S)
LINKER_SCRIPT_FILE = $(realpath lib/h32_core/ld/$(TARGET_DEVICE_LD).ld)

# library files (.a and .o)
LIB_FILES = 

# excluded library files
EXCLUDE_LIB_OBJ_FILES = 

# excluded headers and source files to speed up compiles and reduce possible incompatibilities
# excluded components:
# - HALs other than H32
# - lcd implementations (excluding DWIN, ofc)
# - usb flash drive driver
EXCLUDES = \
	src/HAL/AVR/ \
	src/HAL/DUE/ \
	src/HAL/ESP32/ \
	src/HAL/LINUX/ \
	src/HAL/LPC1768/ \
	src/HAL/NATIVE_SIM/ \
	src/HAL/SAMD21/ \
	src/HAL/SAMD51/ \
	src/HAL/STM32/ \
	src/HAL/STM32F1/ \
	src/HAL/TEENSY31_32/ \
	src/HAL/TEENSY35_36/ \
	src/HAL/TEENSY35_41/ \
	src/lcd/dogm/ \
	src/lcd/extui/ \
	src/lcd/HD44870/ \
	src/lcd/tft/ \
	src/lcd/tft_io/ \
	src/lcd/TFTGLCD/ \
	src/lcd/touch/ \
	src/sd/usb_flashdrive/

## defines for compilation (-D) ##

C_DEFINES = \
	HC32F46x \
	TARGET_HC32F46x \
	TARGET_STM32F1 \
	ARDUINO_ARCH_STM32F1 \
	ARDUINO=100 \
	STM32_HIGH_DENSITY \
	USE_DEVICE_DRIVER_LIB \
	__TARGET_FPU_VFP \
	__FPU_PRESENT=1 \
	_MPU_PRESENT=1 \
	ARM_MATH_CM4 \
	ARM_MATH_MATRIX_CHECK \
	ARM_MATH_ROUNDING \
	REDIRECT_PRINTF_TO_SERIAL \
	__DEBUG

## Compiler Arguments ##
COMMON_GCC_ARGS = \
	-mcpu=cortex-m4 \
	-mthumb \
	-mthumb-interwork \
	-Os \
	-fmessage-length=0 \
	-fsigned-char \
	-ffunction-sections \
	-fdata-sections \
	-Wall \
	-g

## resolve and prepare files ##
BUILD_DIR = $(abspath $(BUILD_DIR_NAME))
FILES_TO_COMPILE_REL = $(call not-containing,$(EXCLUDES),$(C_FILES) $(CPP_FILES) $(ASM_FILES))
FILES_TO_COMPILE = $(realpath $(FILES_TO_COMPILE_REL))
OBJ_FILES = $(abspath $(addsuffix .o,$(addprefix $(BUILD_DIR_NAME)/,$(FILES_TO_COMPILE_REL))))
DEP_FILES = $(OBJ_FILES:.o=.d)
# include all directories containing .h files
INCLUDE_PATHS = $(realpath $(sort $(foreach d,$(call not-containing,$(EXCLUDES),$(call rwildcard,,*.h)),$(dir $d))))
OUTPUT_FILE_BASE = $(abspath $(BUILD_DIR)/firmware)

# resolve library files (object and library)
LIB_FILES_RESOLVED = $(call not-containing,$(EXCLUDE_LIB_OBJ_FILES),$(LIB_FILES))

### Compiler Configuration ###
TOOL_PREFIX = arm-none-eabi-
C_STD = gnu17
CPP_STD = gnu++17

### TARGETS ###
# All
all: $(OUTPUT_FILE_BASE).bin

# C++ Compile
$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo 'Compile $<'
	@mkdir -p $(dir $@)
	@$(TOOL_PREFIX)g++ \
		$(COMMON_GCC_ARGS) \
		$(addprefix -D,$(C_DEFINES)) \
		$(addprefix -I,$(INCLUDE_PATHS)) \
		-std=$(CPP_STD) \
		-fabi-version=0 \
		-MMD \
		-MP \
		-MF"$(@:%.o=%.d)" \
		-MT"$(@)" \
		-c \
		-o "$@" "$<"

# C Compile
$(BUILD_DIR)/%.c.o: %.c
	@echo 'Compile $<'
	@mkdir -p $(dir $@)
	@$(TOOL_PREFIX)gcc \
		$(COMMON_GCC_ARGS) \
		$(addprefix -D,$(C_DEFINES)) \
		$(addprefix -I,$(INCLUDE_PATHS)) \
    	-std=$(C_STD) \
    	-MMD \
    	-MP \
    	-MF"$(@:%.o=%.d)" \
    	-MT"$(@)" \
    	-c \
    	-o "$@" "$<"

# ASM Compile
$(BUILD_DIR)/%.S.o: %.S
	@echo 'Compile $<'
	@mkdir -p $(dir $@)
	@$(TOOL_PREFIX)gcc \
		$(COMMON_GCC_ARGS) \
    	-x assembler-with-cpp \
    	-MMD \
    	-MP \
    	-MF"$(@:%.o=%.d)" \
    	-MT"$(@)" \
    	-c \
    	-o "$@" "$<"

# Link firmware
# RWX segment warnings are disabled since they are not relevant on the H32 SoC
$(OUTPUT_FILE_BASE).elf: $(OBJ_FILES)
	@echo 'Linking Firmware'
	@mkdir -p $(dir $@)
	@$(TOOL_PREFIX)g++ \
		$(COMMON_GCC_ARGS) \
		-L $(dir $(LINKER_SCRIPT_FILE)) \
		-T $(LINKER_SCRIPT_FILE) \
		-Xlinker \
		--gc-sections \
		-Wl,-Map,$(OUTPUT_FILE_BASE).map \
		-Wl,--no-warn-rwx-segment \
		-Wl,--print-memory-usage \
		--specs=nano.specs \
		--specs=nosys.specs \
		-o $(OUTPUT_FILE_BASE).elf $(OBJ_FILES) $(LIB_FILES_RESOLVED)

# Create Flash Image
$(OUTPUT_FILE_BASE).bin: $(OUTPUT_FILE_BASE).elf
	@echo 'Creating Flash Image'
	@mkdir -p $(dir $@)
	@$(TOOL_PREFIX)objcopy -O binary $(OUTPUT_FILE_BASE).elf $(OUTPUT_FILE_BASE).bin

# print segments in the elf file
print-segments: $(OUTPUT_FILE_BASE).elf
	@$(TOOL_PREFIX)readelf -lWS $(OUTPUT_FILE_BASE).elf

# clean
clean:
	@echo 'Cleaning Project'
	@rm -rf $(BUILD_DIR)

# clean, clear console, build all
rebuild: clean
	clear
	@$(MAKE) -f $(realpath $(lastword $(MAKEFILE_LIST))) all

# Print Resolved Files
print-resolved:
	@printf ' == C_FILES == \n $(addsuffix \n,$(C_FILES)) \n\n'
	@printf ' == CPP_FILES == \n $(addsuffix \n,$(CPP_FILES)) \n\n'
	@printf ' == ASM_FILES == \n $(addsuffix \n,$(ASM_FILES)) \n\n'
	@printf ' == OBJ_FILES == \n $(addsuffix \n,$(OBJ_FILES)) \n\n'
	@printf ' == LIB_FILES_RESOLVED == \n $(addsuffix \n,$(LIB_FILES_RESOLVED)) \n\n'
	@printf ' == DEP_FILES == \n $(addsuffix \n,$(DEP_FILES)) \n\n'
	@printf ' == INCLUDE_PATHS == \n $(addsuffix \n,$(INCLUDE_PATHS)) \n\n'
	@printf ' == BUILD_DIR == \n $(addsuffix \n,$(BUILD_DIR)) \n\n'
	@printf ' == OUTPUT_FILE_BASE == \n $(addsuffix \n,$(OUTPUT_FILE_BASE)) \n\n'

.PHONY: all clean rebuild print-segments print-resolved
