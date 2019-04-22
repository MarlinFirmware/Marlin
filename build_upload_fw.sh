sudo mount $(ls /dev/sd*1) /mnt/sdcard
pio run
sudo cp .pioenvs/LPC1768/firmware.bin /mnt/sdcard/
sleep 2
stty speed 115200 < /dev/ttyACM0
stty speed 115200 < /dev/ttyACM0
sleep 2
echo -ne 'M997\n' > /dev/ttyACM0
