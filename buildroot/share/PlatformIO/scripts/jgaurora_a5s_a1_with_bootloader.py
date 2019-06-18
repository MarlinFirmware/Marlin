Import("env")

# Relocate firmware from 0x08000000 to 0x0800A000
env['CPPDEFINES'].remove(("VECT_TAB_ADDR", "0x8000000"))
#alternatively, for STSTM <=5.1.0 use line below
#env['CPPDEFINES'].remove(("VECT_TAB_ADDR", 134217728))
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x0800A000"))
env.Replace(LDSCRIPT_PATH="buildroot/share/PlatformIO/ldscripts/jgaurora_a5s_a1.ld")

#append ${PROGNAME}.bin firmware after bootloader and save it as 'jgaurora_firmware.bin'
def addboot(source,target,env):
	import os

	firmware = open(target[0].path, "rb")
	lengthfirmware = os.path.getsize(target[0].path)
	bootloader_dir = "buildroot/share/PlatformIO/scripts/jgaurora_bootloader.bin"
	bootloader = open(bootloader_dir, "rb")
	lengthbootloader = os.path.getsize(bootloader_dir)
	firmware_with_boothloader_dir = target[0].dir.path +'/firmware_with_bootloader.bin'
	if os.path.exists(firmware_with_boothloader_dir):
		os.remove(firmware_with_boothloader_dir)
	firmwareimage = open(firmware_with_boothloader_dir, "wb")
	position = 0
	while position < lengthbootloader:
		byte = bootloader.read(1)
		firmwareimage.write(byte)
		position += 1
	position = 0
	while position < lengthfirmware:
		byte = firmware.read(1)
		firmwareimage.write(byte)
		position += 1
	bootloader.close()
	firmware.close()
	firmwareimage.close()
	firmware_without_bootloader_dir = target[0].dir.path+'/firmware_for_sd_upload.bin'
	if os.path.exists(firmware_without_bootloader_dir):
		os.remove(firmware_without_bootloader_dir)
	os.rename(target[0].path, firmware_without_bootloader_dir)
	#os.rename(target[0].dir.path+'/firmware_with_bootloader.bin', target[0].dir.path+'/firmware.bin')

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", addboot);

