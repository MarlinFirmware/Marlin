#!/bin/bash
USBDEV=`ssh hypercube 'ls -1d /dev/sd?1'`
echo "Mounting remote MMC card ..." && \
    ssh hypercube 'if [ ! -f "/mnt/FIRMWARE.CUR" ]; then sudo mount -o uid=1000' $USBDEV '/mnt ; fi' && \
    echo "Copying firmware file ..." && \
    scp .pio/build/LPC1768/firmware.bin hypercube:/mnt/firmware.bin && \
    echo "Waiting for disk write to complete ..." && \
    ssh hypercube 'sync && sudo umount /mnt' &&
    echo "Rebooting printer ..." && \
    ssh hypercube 'gpio write 8 1 ; sleep 3 ; gpio write 8 0' &&
    echo "Done."


