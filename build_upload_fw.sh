ssh  pi@192.168.1.132 -t '''sudo mount $(ls /dev/sd*1) /mnt/sdcard -o umask=000'''
pio run --disable-auto-clean
scp .pioenvs/LPC1768/firmware.bin pi@192.168.1.132:/mnt/sdcard
ssh  pi@192.168.1.132 -t "stty speed 115200 < /dev/ttyACM0 ; stty speed 115200 < /dev/ttyACM0 ; sleep 2; echo -ne 'M997\n' > /dev/ttyACM0"

