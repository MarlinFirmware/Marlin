#
# sets output_port
#  if target_filename is found then that drive is used
#  else if target_drive is found then that drive is used
#
from __future__ import print_function

target_filename = "FIRMWARE.CUR"
target_drive = "REARM"

import os,getpass,platform

current_OS = platform.system()
Import("env")

def print_error(e):
	print('\nUnable to find destination disk (%s)\n' \
		  'Please select it in platformio.ini using the upload_port keyword ' \
		  '(https://docs.platformio.org/en/latest/projectconf/section_env_upload.html) ' \
		  'or copy the firmware (.pio/build/%s/firmware.bin) manually to the appropriate disk\n' \
		  %(e, env.get('PIOENV')))

try:
	#
	# Find a disk for upload
	#
	upload_disk = 'Disk not found'
	target_file_found = False
	target_drive_found = False
	if current_OS == 'Windows':
		#
		# platformio.ini will accept this for a Windows upload port designation: 'upload_port = L:'
		#   Windows - doesn't care about the disk's name, only cares about the drive letter
		import subprocess,string
		from ctypes import windll

		# getting list of drives
		# https://stackoverflow.com/questions/827371/is-there-a-way-to-list-all-the-available-drive-letters-in-python
		drives = []
		bitmask = windll.kernel32.GetLogicalDrives()
		for letter in string.ascii_uppercase:
			if bitmask & 1:
				drives.append(letter)
			bitmask >>= 1

		for drive in drives:
			final_drive_name = drive + ':\\'
			# print ('disc check: {}'.format(final_drive_name))
			try:
				volume_info = str(subprocess.check_output('cmd /C dir ' + final_drive_name, stderr=subprocess.STDOUT))
			except Exception as e:
				print ('error:{}'.format(e))
				continue
			else:
				if target_drive in volume_info and not target_file_found:  # set upload if not found target file yet
					target_drive_found = True
					upload_disk = final_drive_name
				if target_filename in volume_info:
					if not target_file_found:
						upload_disk = final_drive_name
					target_file_found = True

	elif current_OS == 'Linux':
		#
		# platformio.ini will accept this for a Linux upload port designation: 'upload_port = /media/media_name/drive'
		#
		drives = os.listdir(os.path.join(os.sep, 'media', getpass.getuser()))
		if target_drive in drives:  # If target drive is found, use it.
			target_drive_found = True
			upload_disk = os.path.join(os.sep, 'media', getpass.getuser(), target_drive) + os.sep
		else:
			for drive in drives:
				try:
					files = os.listdir(os.path.join(os.sep, 'media', getpass.getuser(), drive))
				except:
					continue
				else:
					if target_filename in files:
						upload_disk = os.path.join(os.sep, 'media', getpass.getuser(), drive) + os.sep
						target_file_found = True
						break
		#
		# set upload_port to drive if found
		#

		if target_file_found or target_drive_found:
			env.Replace(
				UPLOAD_FLAGS="-P$UPLOAD_PORT"
			)

	elif current_OS == 'Darwin':  # MAC
		#
		# platformio.ini will accept this for a OSX upload port designation: 'upload_port = /media/media_name/drive'
		#
		drives = os.listdir('/Volumes')  # human readable names
		if target_drive in drives and not target_file_found:  # set upload if not found target file yet
			target_drive_found = True
			upload_disk = '/Volumes/' + target_drive + '/'
		for drive in drives:
			try:
				filenames = os.listdir('/Volumes/' + drive + '/')   # will get an error if the drive is protected
			except:
				continue
			else:
				if target_filename in filenames:
					if not target_file_found:
						upload_disk = '/Volumes/' + drive + '/'
					target_file_found = True

	#
	# Set upload_port to drive if found
	#
	if target_file_found or target_drive_found:
		env.Replace(UPLOAD_PORT=upload_disk)
		print('\nUpload disk: ', upload_disk, '\n')
	else:
		print_error('Autodetect Error')

except Exception as e:
	print_error(str(e))
