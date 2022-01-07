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
Debug = True

#-----------------#
# Upload Callback #
#-----------------#
def Upload(source, target, env):

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
    def _GetFirmwareFiles():
        if Debug: print('Get firmware files...')
        _Send('M20 F')
        Responses = _Recv()
        if len(Responses) < 3 or not any('file list' in r for r in Responses):
            raise Exception('Error getting firmware files')
        if Debug: print('OK')
        return Responses
        
    def _FilterFirmwareFiles(FirmwareList):
        Firmwares = []
        for FWFile in FirmwareList:
            if not '/' in FWFile and '.BIN' in FWFile:
                idx = FWFile.index('.BIN')
                Firmwares.append(FWFile[:idx+4])
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
    protocol = None
    filetransfer = None
    # Get firmware upload params
    upload_firmware_source_name = str(source[0])    # Source firmware filename
    upload_speed = env['UPLOAD_SPEED'] if 'UPLOAD_SPEED' in env else 115200
                                                    # baud rate of serial connection
    upload_port = _GetUploadPort(env)               # Serial port to use
    # Set local upload params
    #upload_firmware_target_name = os.path.basename(upload_firmware_source_name)    # Need rework on "binary_stream" to allow filename > 8.3
    upload_firmware_target_name = f"fw-{''.join(random.choices('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789', k=5))}.BIN"
                                                    # Target firmware filename (randomly generated)
    upload_timeout = 1000                           # Communication timout, lossy/slow connections need higher values
    upload_blocksize = 512                          # Transfer block size. 512 = Autodetect
    upload_compression = True                       # Enable compression
    upload_error_ratio = 0                          # Simulated corruption ratio
    upload_test = False                             # Benchmark the serial link without storing the file
    upload_reset = True                             # Trigger a soft reset for firmware update after the upload

    if Debug:
        print('Upload using:')
        print(f' -Source      : {upload_firmware_source_name}')
        print(f' -Target      : {upload_firmware_target_name}')
        print(f' -Port        : {upload_port} @ {upload_speed} baudrate')
        print(f' -Timeout     : {upload_timeout}')
        print(f' -Block size  : {upload_blocksize}')
        print(f' -Compression : {upload_compression}')
        print(f' -Error ratio : {upload_error_ratio}')
        print(f' -Test        : {upload_test}')
        print(f' -Reset       : {upload_reset}')
        print('----------------')

    try:
        # Init serial port
        port = serial.Serial(upload_port, baudrate = upload_speed, write_timeout = 0, timeout = 0.1)
        port.reset_input_buffer()

        # Check SD card status
        _CheckSDCard()
        
        # Get firmware files
        FirmwareFiles = _GetFirmwareFiles()
        if Debug:
            for FirmwareFile in FirmwareFiles:
                print(f'Found: {FirmwareFile}')
            
        # Get all 1st level firmware files (to remove)
        OldFirmwareFiles = _FilterFirmwareFiles(FirmwareFiles[1:len(FirmwareFiles)-2])   # Skip header and footers of list
        if len(OldFirmwareFiles) == 0:
            print('No old firmware files found')
        else:
            print(f"Remove {len(OldFirmwareFiles)} old firmware file{'s' if len(OldFirmwareFiles) != 1 else ''}:")
            for OldFirmwareFile in OldFirmwareFiles:
                print(f" -Removing- '{OldFirmwareFile}'...")
                print(' OK' if _RemoveFirmwareFile(OldFirmwareFile) else ' Error!')

        # Cleanup completed
        port.close()
        if Debug: print('Cleanup completed')
    
        # Upload File
        print(f"Copying '{upload_firmware_source_name}' --> '{upload_firmware_target_name}'")
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

        if upload_reset:
            print('Trigger firmware update...')
            protocol.send_ascii('M997', True)

        print('Firmware upload completed')

    except KeyboardInterrupt:
        if filetransfer: filetransfer.abort()
        if protocol: protocol.shutdown()
        raise

    except MarlinBinaryProtocol.FatalError:
        if protocol: protocol.shutdown()
        print('Too many retries, Abort')
        raise

    except serial.SerialException as se:
        print(f'Serial excepion: {se}')
        raise Exception(se)

    except:
        print('Firmware not updated')
        if filetransfer: filetransfer.abort()
        if protocol: protocol.shutdown()
        raise

# Attach custom upload callback
env.Replace(UPLOADCMD=Upload)
