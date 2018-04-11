#
# sets output_port
#  if target_filename is found then that drive is used
#  else if target_drive is found then that drive is used
#

target_filename = "FIRMWARE.CUR"
target_drive = "REARM"


import platform
current_OS = platform.system()

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

    #
    # scan top directory of each drive for FIRMWARE.CUR
    #   return first drive found
    #

    import os
    target_file_found = False
    target_drive_found = False
    for drive in drives:
      final_drive_name = drive.strip().rstrip('\\')   # typical result (string): 'C:'
      # modified version of walklevel()
      level=0
      some_dir = "/"
      some_dir = some_dir.rstrip(os.path.sep)
      assert os.path.isdir(some_dir)
      num_sep = some_dir.count(os.path.sep)
      for root, dirs, files in os.walk(final_drive_name):
        num_sep_this = root.count(os.path.sep)
        if num_sep + level <= num_sep_this:
          del dirs[:]
        volume_info = subprocess.check_output('fsutil fsinfo volumeinfo ' + final_drive_name)
        if target_drive in volume_info and target_file_found == False:  # set upload if not found target file yet
          target_drive_found = True
          upload_disk = root
        if target_filename in files:
          if target_file_found == False:
            upload_disk = root
          target_file_found = True

    #
    # set upload_port to drive if found
    #

    if target_file_found == True or target_drive_found == True:
      Import("env")
      env.Replace(
          UPLOAD_PORT = upload_disk
      )



if current_OS == 'Linux':

    #
    # platformio.ini will accept this for a Linux upload port designation: 'upload_port = /media/media_name/drive'
    #

    import os
    target_file_found = False
    target_drive_found = False
    medias = os.listdir('/media')  #
    for media in medias:
      drives = os.listdir('/media/' + media)  #
      if target_drive in drives and target_file_found == False:  # set upload if not found target file yet
        target_drive_found = True
        upload_disk = '/media/' + media + '/' + target_drive + '/'
      for drive in drives:
        files = os.listdir('/media/' + media + '/' + drive )  #
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


if current_OS == 'Darwin':  # MAC

    #
    # platformio.ini will accept this for a OSX upload port designation: 'upload_port = /media/media_name/drive'
    #

    import os
    drives = os.listdir('/Volumes')  #  human readable names
    target_file_found = False
    target_drive_found = False
    if target_drive in drives and target_file_found == False:  # set upload if not found target file yet
      target_drive_found = True
      upload_disk = '/Volumes/' + drive + '/'
    for drive in drives:
      target_file_found = True
      filenames = os.listdir('/Volumes/' + drive + '/')
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
