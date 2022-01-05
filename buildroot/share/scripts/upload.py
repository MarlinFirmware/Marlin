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

import MarlinBinaryProtocol

# Internal debug flag
Debug = False

#-----------------#
# Upload Callback #
#-----------------#
def Upload(source, target, env):

    #----------------#
    # Port functions #
    #----------------#
    def _GetUploadPort(env):
        if Debug: print("Autodetecting upload port...")
        env.AutodetectUploadPort(env)
        port = env.subst('$UPLOAD_PORT')
        if not port:
            print("Error detecting the upload port.")
            return None
        if Debug: print('OK')
        return port

    #-------------------------#
    # Simple serial functions #
    #-------------------------#
    def _Send(data):
        if Debug: print(">> " + data)
        strdata = bytearray(data, "utf8") + b'\n'
        port.write(strdata)
        time.sleep(0.010)

    def _Recv():
        clean_responses = []
        responses = port.readlines()
        for Resp in responses:
            clean_response = Resp.decode('utf8').rstrip().lstrip()
            clean_responses.append(clean_response)
            if Debug: print("<< " + clean_response)
        return clean_responses
        
    #------------------#
    # SDCard functions #
    #------------------#
    def _CheckSDCard():
        if Debug: print('Checking SD card...')
        _Send("M21")
        Responses = _Recv()
        if len(Responses) < 1 or not 'SD card ok' in Responses[0]:
            print("Error accessing SD card")
            return False
        if Debug: print('SD Card OK')
        return True

    #----------------#
    # File functions #
    #----------------#
    def _GetFirmwareFiles():
        if Debug: print('Get firmware files...')
        _Send("M20 F")
        Responses = _Recv()
        if len(Responses) < 3 or not 'file list' in Responses[0]:
            print("Error getting firmware files")
            return (False, None)
        if Debug: print('OK')
        return (True, Responses)
        
    def _FilterFirmwareFiles(FirmwareList):
        Firmwares = []
        for FWFile in FirmwareList:
            if not "/" in FWFile and ".BIN" in FWFile:
                idx = FWFile.index(".BIN")
                Firmwares.append(FWFile[:idx+4])
        return Firmwares

    def _RemoveFirmwareFile(FWFile):
        _Send("M30 /" + FWFile)
        Responses = _Recv()
        return len(Responses) >= 1 and 'File deleted' in Responses[0]


    #---------------------#
    # Callback Entrypoint #
    #---------------------#
    # Get firmware upload params
    upload_firmware_source_name = str(source[0])    # Source firmware filename
    upload_speed = env['UPLOAD_SPEED'] if 'UPLOAD_SPEED' in env else 115200
                                                    # baud rate of serial connection
    upload_port = _GetUploadPort(env)               # Serial port to use
    if not upload_port: exit(1)
    # Set local upload params
    upload_firmware_target_name = 'fw-' + ''.join(random.choices("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", k=5)) + ".BIN"
                                                    # Target firmware filename (randomly generated)
    upload_timeout = 1000                           # Communication timout, lossy/slow connections need higher values
    upload_blocksize = 512                          # Transfer block size. 512 = Autodetect
    upload_compression = True                       # Enable compression
    upload_error_ratio = 0                          # Simulated corruption ratio
    upload_test = False                             # Benchmark the serial link without storing the file
    upload_reset = True                             # Trigger a soft reset for firmware update after the upload

    if Debug:
        print("Upload using:")
        print(f" -Source      : {upload_firmware_source_name}")
        print(f" -Target      : {upload_firmware_target_name}")
        print(f" -Port        : {upload_port} @ {upload_speed} baudrate")
        print(f" -Timeout     : {upload_timeout}")
        print(f" -Block size  : {upload_blocksize}")
        print(f" -Compression : {upload_compression}")
        print(f" -Error ratio : {upload_error_ratio}")
        print(f" -Test        : {upload_test}")
        print(f" -Reset       : {upload_reset}")
        print("----------------")

    # Init serial port
    port = serial.Serial(upload_port, baudrate = upload_speed, write_timeout = 0, timeout = 0.1)
    port.reset_input_buffer()

    # Check SD card status
    if not _CheckSDCard(): exit(2)
    
    # Get firmware files
    (Status, FirmwareFiles) = _GetFirmwareFiles()
    if not Status: exit(3)
    if Debug:
        for FirmwareFile in FirmwareFiles:
            print("Found: " + FirmwareFile)
        
    # Get all 1st level firmware files (to remove)
    OldFirmwareFiles = _FilterFirmwareFiles(FirmwareFiles[1:len(FirmwareFiles)-2])   # Skip header and footers of list
    if len(OldFirmwareFiles) == 0:
        print("No old firmware files found")
    else:
        print(f"Remove {len(OldFirmwareFiles)} old firmware file{'s' if len(OldFirmwareFiles) != 1 else ''}:")
        for OldFirmwareFile in OldFirmwareFiles:
            print(" -Removing- " + OldFirmwareFile + "...", end='', flush=True)
            print("OK" if _RemoveFirmwareFile(OldFirmwareFile) else "Error!")

    # Cleanup completed
    port.close()
    if Debug: print("Cleanup completed")
    
    try:
        # Upload File
        print("Copying '" + upload_firmware_source_name + "' --> '" + upload_firmware_target_name + "'")
        protocol = MarlinBinaryProtocol.Protocol(upload_port, upload_speed, upload_blocksize, float(upload_error_ratio), int(upload_timeout))
        #echologger = MarlinBinaryProtocol.EchoProtocol(protocol)
        protocol.connect()
        filetransfer = MarlinBinaryProtocol.FileTransferProtocol(protocol)
        filetransfer.copy(upload_firmware_source_name, upload_firmware_target_name, upload_compression, upload_test)
        protocol.disconnect()

        # Notify upload completed
        protocol.send_ascii("M117 Firmware uploaded")

        # Remount SD card
        print('Wait for SD card release...')
        time.sleep(1)
        print('Remount SD card')
        protocol.send_ascii("M21")

        if upload_reset:
            print("Trigger firmware update...")
            protocol.send_ascii("M997", True)
    except KeyboardInterrupt:
        if filetransfer: filetransfer.abort()

    except MarlinBinaryProtocol.FatalError:
        print("Too Many Retries, Abort")

    finally:
        if protocol: protocol.shutdown()
        print("Firmware upload completed")
    

# Attach custom upload callback
env.Replace(UPLOADCMD=Upload)
