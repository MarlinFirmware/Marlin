import argparse
import sys
import os
import time
import random
import serial

Import("env")

# Needed (only) for compression, but there are problems with pip install heatshrink
#try:
#    import heatshrink
#except ImportError:
#    # Install heatshrink
#    print("Installing 'heatshrink' python module...")
#    env.Execute(env.subst("$PYTHONEXE -m pip install heatshrink"))
#
# Not tested: If it's safe to install python libraries in PIO python try:
#    env.Execute(env.subst("$PYTHONEXE -m pip install https://github.com/p3p/pyheatshrink/releases/download/0.3.3/pyheatshrink-pip.zip"))

import MarlinBinaryProtocol

# Internal debug flag
Debug = False

#-----------------#
# Upload Callback #
#-----------------#
def Upload(source, target, env):

    #------------------#
    # Marlin functions #
    #------------------#
    def _GetMarlinEnv(marlinEnv, feature):
        if not marlinEnv: return None
        return marlinEnv[feature] if feature in marlinEnv else None

    #----------------#
    # Port functions #
    #----------------#
    def _GetUploadPort(env):
        if Debug: print('Autodetecting upload port...')
        env.AutodetectUploadPort(env)
        port = env.subst('$UPLOAD_PORT')
        if not port:
            raise Exception('Error detecting the upload port.')
        if Debug: print('OK')
        return port

    #-------------------------#
    # Simple serial functions #
    #-------------------------#
    def _Send(data):
        if Debug: print(f'>> {data}')
        strdata = bytearray(data, 'utf8') + b'\n'
        port.write(strdata)
        time.sleep(0.010)

    def _Recv():
        clean_responses = []
        responses = port.readlines()
        for Resp in responses:
            # Test: suppress invaid chars (coming from debug info)
            try:
                clean_response = Resp.decode('utf8').rstrip().lstrip()
                clean_responses.append(clean_response)
            except:
                pass
            if Debug: print(f'<< {clean_response}')
        return clean_responses

    #------------------#
    # SDCard functions #
    #------------------#
    def _CheckSDCard():
        if Debug: print('Checking SD card...')
        _Send('M21')
        Responses = _Recv()
        if len(Responses) < 1 or not any('SD card ok' in r for r in Responses):
            raise Exception('Error accessing SD card')
        if Debug: print('SD Card OK')
        return True

    #----------------#
    # File functions #
    #----------------#
    def _GetFirmwareFiles(UseLongFilenames):
        if Debug: print('Get firmware files...')
        _Send(f"M20 F{'L' if UseLongFilenames else ''}")
        Responses = _Recv()
        if len(Responses) < 3 or not any('file list' in r for r in Responses):
            raise Exception('Error getting firmware files')
        if Debug: print('OK')
        return Responses

    def _FilterFirmwareFiles(FirmwareList, UseLongFilenames):
        Firmwares = []
        for FWFile in FirmwareList:
            # For long filenames take the 3rd column of the firmwares list
            if UseLongFilenames:
                Space = 0
                Space = FWFile.find(' ')
                if Space >= 0: Space = FWFile.find(' ', Space + 1)
                if Space >= 0: FWFile = FWFile[Space + 1:]
            if not '/' in FWFile and '.BIN' in FWFile.upper():
                Firmwares.append(FWFile[:FWFile.upper().index('.BIN') + 4])
        return Firmwares

    def _RemoveFirmwareFile(FirmwareFile):
        _Send(f'M30 /{FirmwareFile}')
        Responses = _Recv()
        Removed = len(Responses) >= 1 and any('File deleted' in r for r in Responses)
        if not Removed:
            raise Exception(f"Firmware file '{FirmwareFile}' not removed")
        return Removed


    #---------------------#
    # Callback Entrypoint #
    #---------------------#
    port = None
    protocol = None
    filetransfer = None

    # Get Marlin evironment vars
    MarlinEnv = env['MARLIN_FEATURES']
    marlin_pioenv = _GetMarlinEnv(MarlinEnv, 'PIOENV')
    marlin_motherboard = _GetMarlinEnv(MarlinEnv, 'MOTHERBOARD')
    marlin_board_info_name = _GetMarlinEnv(MarlinEnv, 'BOARD_INFO_NAME')
    marlin_board_custom_build_flags = _GetMarlinEnv(MarlinEnv, 'BOARD_CUSTOM_BUILD_FLAGS')
    marlin_firmware_bin = _GetMarlinEnv(MarlinEnv, 'FIRMWARE_BIN')
    marlin_long_filename_host_support = _GetMarlinEnv(MarlinEnv, 'LONG_FILENAME_HOST_SUPPORT') is not None
    marlin_longname_write = _GetMarlinEnv(MarlinEnv, 'LONG_FILENAME_WRITE_SUPPORT') is not None
    marlin_custom_firmware_upload = _GetMarlinEnv(MarlinEnv, 'CUSTOM_FIRMWARE_UPLOAD') is not None
    marlin_short_build_version = _GetMarlinEnv(MarlinEnv, 'SHORT_BUILD_VERSION')
    marlin_string_config_h_author = _GetMarlinEnv(MarlinEnv, 'STRING_CONFIG_H_AUTHOR')

    # Get firmware upload params
    upload_firmware_source_name = str(source[0])    # Source firmware filename
    upload_speed = env['UPLOAD_SPEED'] if 'UPLOAD_SPEED' in env else 115200
                                                    # baud rate of serial connection
    upload_port = _GetUploadPort(env)               # Serial port to use

    # Set local upload params
    upload_firmware_target_name = os.path.basename(upload_firmware_source_name)     # WARNING! Need rework on "binary_stream" to allow filename > 8.3
                                                    # Target firmware filename
    upload_timeout = 1000                           # Communication timout, lossy/slow connections need higher values
    upload_blocksize = 512                          # Transfer block size. 512 = Autodetect
    upload_compression = True                       # Enable compression
    upload_error_ratio = 0                          # Simulated corruption ratio
    upload_test = False                             # Benchmark the serial link without storing the file
    upload_reset = True                             # Trigger a soft reset for firmware update after the upload

    # Set local upload params based on board type to change script behavior
    # "upload_delete_old_bins": delete all *.bin files in the root of SD Card
    upload_delete_old_bins = marlin_motherboard in ['BOARD_CREALITY_V4',   'BOARD_CREALITY_V4210', 'BOARD_CREALITY_V422', 'BOARD_CREALITY_V423',
                                                    'BOARD_CREALITY_V427', 'BOARD_CREALITY_V431', 'BOARD_CREALITY_V452',  'BOARD_CREALITY_V453',
                                                    'BOARD_CREALITY_V24S1']
    # "upload_random_name": generate a random 8.3 firmware filename to upload
    upload_random_filename = marlin_motherboard in ['BOARD_CREALITY_V4',   'BOARD_CREALITY_V4210', 'BOARD_CREALITY_V422', 'BOARD_CREALITY_V423',
                                                    'BOARD_CREALITY_V427', 'BOARD_CREALITY_V431', 'BOARD_CREALITY_V452',  'BOARD_CREALITY_V453',
                                                    'BOARD_CREALITY_V24S1'] and not marlin_long_filename_host_support

    try:

        # Start upload job
        print(f"Uploading firmware '{os.path.basename(upload_firmware_target_name)}' to '{marlin_motherboard}' via '{upload_port}'")

        # Dump some debug info
        if Debug:
            print('Upload using:')
            print('---- Marlin -----------------------------------')
            print(f' PIOENV                      : {marlin_pioenv}')
            print(f' SHORT_BUILD_VERSION         : {marlin_short_build_version}')
            print(f' STRING_CONFIG_H_AUTHOR      : {marlin_string_config_h_author}')
            print(f' MOTHERBOARD                 : {marlin_motherboard}')
            print(f' BOARD_INFO_NAME             : {marlin_board_info_name}')
            print(f' CUSTOM_BUILD_FLAGS          : {marlin_board_custom_build_flags}')
            print(f' FIRMWARE_BIN                : {marlin_firmware_bin}')
            print(f' LONG_FILENAME_HOST_SUPPORT  : {marlin_long_filename_host_support}')
            print(f' LONG_FILENAME_WRITE_SUPPORT : {marlin_longname_write}')
            print(f' CUSTOM_FIRMWARE_UPLOAD      : {marlin_custom_firmware_upload}')
            print('---- Upload parameters ------------------------')
            print(f' Source                      : {upload_firmware_source_name}')
            print(f' Target                      : {upload_firmware_target_name}')
            print(f' Port                        : {upload_port} @ {upload_speed} baudrate')
            print(f' Timeout                     : {upload_timeout}')
            print(f' Block size                  : {upload_blocksize}')
            print(f' Compression                 : {upload_compression}')
            print(f' Error ratio                 : {upload_error_ratio}')
            print(f' Test                        : {upload_test}')
            print(f' Reset                       : {upload_reset}')
            print('-----------------------------------------------')

        # Custom implementations based on board parameters
        # Generate a new 8.3 random filename
        if upload_random_filename:
            upload_firmware_target_name = f"fw-{''.join(random.choices('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789', k=5))}.BIN"
            print(f"Board {marlin_motherboard}: Overriding firmware filename to '{upload_firmware_target_name}'")

        # Delete all *.bin files on the root of SD Card (if flagged)
        if upload_delete_old_bins:
            # CUSTOM_FIRMWARE_UPLOAD is needed for this feature
            if not marlin_custom_firmware_upload:
                raise Exception(f"CUSTOM_FIRMWARE_UPLOAD must be enabled in 'Configuration_adv.h' for '{marlin_motherboard}'")

            # Init serial port
            port = serial.Serial(upload_port, baudrate = upload_speed, write_timeout = 0, timeout = 0.1)
            port.reset_input_buffer()

            # Check SD card status
            _CheckSDCard()

            # Get firmware files
            FirmwareFiles = _GetFirmwareFiles(marlin_long_filename_host_support)
            if Debug:
                for FirmwareFile in FirmwareFiles:
                    print(f'Found: {FirmwareFile}')

            # Get all 1st level firmware files (to remove)
            OldFirmwareFiles = _FilterFirmwareFiles(FirmwareFiles[1:len(FirmwareFiles)-2], marlin_long_filename_host_support)   # Skip header and footers of list
            if len(OldFirmwareFiles) == 0:
                print('No old firmware files to delete')
            else:
                print(f"Remove {len(OldFirmwareFiles)} old firmware file{'s' if len(OldFirmwareFiles) != 1 else ''}:")
                for OldFirmwareFile in OldFirmwareFiles:
                    print(f" -Removing- '{OldFirmwareFile}'...")
                    print(' OK' if _RemoveFirmwareFile(OldFirmwareFile) else ' Error!')

            # Close serial
            port.close()

            # Cleanup completed
            if Debug: print('Cleanup completed')

        # WARNING! The serial port must be closed here because the serial transfer that follow needs it!

        # Upload firmware file
        if Debug: print(f"Copy '{upload_firmware_source_name}' --> '{upload_firmware_target_name}'")
        protocol = MarlinBinaryProtocol.Protocol(upload_port, upload_speed, upload_blocksize, float(upload_error_ratio), int(upload_timeout))
        #echologger = MarlinBinaryProtocol.EchoProtocol(protocol)
        protocol.connect()
        filetransfer = MarlinBinaryProtocol.FileTransferProtocol(protocol)
        filetransfer.copy(upload_firmware_source_name, upload_firmware_target_name, upload_compression, upload_test)
        protocol.disconnect()

        # Notify upload completed
        protocol.send_ascii('M117 Firmware uploaded')

        # Remount SD card
        print('Wait for SD card release...')
        time.sleep(1)
        print('Remount SD card')
        protocol.send_ascii('M21')

        # Trigger firmware update
        if upload_reset:
            print('Trigger firmware update...')
            protocol.send_ascii('M997', True)

        protocol: protocol.shutdown()
        print('Firmware update completed')

    except KeyboardInterrupt:
        if port: port.close()
        if filetransfer: filetransfer.abort()
        if protocol: protocol.shutdown()
        raise

    except serial.SerialException as se:
        if port: port.close()
        print(f'Serial excepion: {se}')
        raise Exception(se)

    except MarlinBinaryProtocol.FatalError:
        if port: port.close()
        if protocol: protocol.shutdown()
        print('Too many retries, Abort')
        raise

    except:
        if port: port.close()
        if protocol: protocol.shutdown()
        print('Firmware not updated')
        raise

# Attach custom upload callback
env.Replace(UPLOADCMD=Upload)
