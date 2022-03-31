#
# jgaurora_a5s_a1_with_bootloader.py
# Customizations for env:jgaurora_a5s_a1
#
import pioutil
if pioutil.is_pio_build():
	import os,marlin
	# Append ${PROGNAME}.bin firmware after bootloader and save it as 'jgaurora_firmware.bin'
	def addboot(source, target, env):
		firmware = open(target[0].path, "rb")
		lengthfirmware = os.path.getsize(target[0].path)
		bootloader_bin = "buildroot/share/PlatformIO/scripts/" + "jgaurora_bootloader.bin"
		bootloader = open(bootloader_bin, "rb")
		lengthbootloader = os.path.getsize(bootloader_bin)

		firmware_with_boothloader_bin = target[0].dir.path + '/firmware_with_bootloader.bin'
		if os.path.exists(firmware_with_boothloader_bin):
			os.remove(firmware_with_boothloader_bin)
		firmwareimage = open(firmware_with_boothloader_bin, "wb")
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

		firmware_without_bootloader_bin = target[0].dir.path + '/firmware_for_sd_upload.bin'
		if os.path.exists(firmware_without_bootloader_bin):
			os.remove(firmware_without_bootloader_bin)
		os.rename(target[0].path, firmware_without_bootloader_bin)
		#os.rename(target[0].dir.path+'/firmware_with_bootloader.bin', target[0].dir.path+'/firmware.bin')

	marlin.add_post_action(addboot);
