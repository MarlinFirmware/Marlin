#
# sets output_port
#  if target_filename is found then that drive is used
#  else if target_drive is found then that drive is used
#

target_filename = "FIRMWARE.CUR"
target_drive = "REARM"

import os
import platform
current_OS = platform.system()

#env_vars = subprocess.check_output('platformio run -t envdump')
#env_vars = env_vars.split('\n')
#for env in env_vars:
#  print env
#exit(0)

build_type = os.environ.get("BUILD_TYPE", 'Not Set')
if build_type == 'upload' or build_type == 'traceback' or build_type == 'Not Set' :

    if current_OS == 'Windows':

        #
        # platformio.ini will accept this for a Windows upload port designation: 'upload_port = L:'
        #   Windows - doesn't care about the disk's name, only cares about the drive letter
        #

        #
        # get all drives on this computer
        #

        import subprocess

        driveStr = subprocess.check_output("fsutil fsinfo drives")  # typical result (string): 'Drives: C:\ D:\ E:\ F:\ G:\ H:\ I:\ J:\ K:\ L:\ M:\ Y:\ Z:\'
        driveStr = driveStr.strip().lstrip('Drives: ')  # typical result (string): 'C:\ D:\ E:\ F:\ G:\ H:\ I:\ J:\ K:\ L:\ M:\ Y:\ Z:\'
        drives = driveStr.split()  # typical result (array of stings): ['C:\\', 'D:\\', 'E:\\', 'F:\\', 'G:\\', 'H:\\', 'I:\\', 'J:\\', 'K:\\', 'L:\\', 'M:\\', 'Y:\\', 'Z:\\']

        upload_disk = 'Disk not found'
        target_file_found = False
        target_drive_found = False
        for drive in drives:
          final_drive_name = drive.strip().rstrip('\\')   # typical result (string): 'C:'
          try:
            volume_info = subprocess.check_output('cmd /C dir ' + final_drive_name, stderr=subprocess.STDOUT)
          except Exception as e:
            continue
          else:
            if target_drive in volume_info and target_file_found == False:  # set upload if not found target file yet
              target_drive_found = True
              upload_disk = final_drive_name
            if target_filename in volume_info:
              if target_file_found == False:
                upload_disk = final_drive_name
              target_file_found = True

        #
        # set upload_port to drive if found
        #

        if target_file_found == True or target_drive_found == True:
          Import("env")
          env.Replace(
              UPLOAD_PORT = upload_disk
          )
          print 'upload disk: ' , upload_disk
        else:
          print '\nUnable to find destination disk.  File must be copied manually. \n'


    if current_OS == 'Linux':

        #
        # platformio.ini will accept this for a Linux upload port designation: 'upload_port = /media/media_name/drive'
        #

        upload_disk = 'Disk not found'
        target_file_found = False
        target_drive_found = False
        medias = os.listdir('/media')  #
        for media in medias:
          drives = os.listdir('/media/' + media)  #
          if target_drive in drives and target_file_found == False:  # set upload if not found target file yet
            target_drive_found = True
            upload_disk = '/media/' + media + '/' + target_drive + '/'
          for drive in drives:
            try:
              files = os.listdir('/media/' + media + '/' + drive )
            except:
              continue
            else:
              if target_filename in files:
                if target_file_found == False:
                  upload_disk = '/media/' + media + '/' + drive + '/'
                  target_file_found = True

        #
        # set upload_port to drive if found
        #

        if target_file_found == True or target_drive_found == True:
          Import("env")
          env.Replace(
            UPLOAD_FLAGS = "-P$UPLOAD_PORT",
            UPLOAD_PORT = upload_disk
          )
          print 'upload disk: ' , upload_disk
        else:
           print '\nUnable to find destination disk.  File must be copied manually. \n'


    if current_OS == 'Darwin':  # MAC

        #
        # platformio.ini will accept this for a OSX upload port designation: 'upload_port = /media/media_name/drive'
        #

        import os
        upload_disk = 'Disk not found'
        drives = os.listdir('/Volumes')  #  human readable names
        target_file_found = False
        target_drive_found = False
        if target_drive in drives and target_file_found == False:  # set upload if not found target file yet
          target_drive_found = True
          upload_disk = '/Volumes/' + target_drive + '/'
        for drive in drives:
          try:
            filenames = os.listdir('/Volumes/' + drive + '/')   # will get an error if the drive is protected
          except:
            continue
          else:
            if target_filename in filenames:
              if target_file_found == False:
                upload_disk = '/Volumes/' + drive + '/'
              target_file_found = True
        #
        # set upload_port to drive if found
        #

        if target_file_found == True or target_drive_found == True:
          Import("env")
          env.Replace(
              UPLOAD_PORT = upload_disk
          )
          print '\nupload disk: ' , upload_disk, '\n'
        else:
           print '\nUnable to find destination disk.  File must be copied manually. \n'
