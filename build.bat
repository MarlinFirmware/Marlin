platformio run -e NUCLEO_F746ZG_with_bootloader
cd C:\work\debug\ARM_tools
copy C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\.pio\build\NUCLEO_F746ZG_with_bootloader\firmware.elf
.\objdump.exe -d -S -l -C -t firmware.elf >C:\work\debug\ARM_tools\ARM_disassemble_with_line_numbers.txt
cd C:\Users\bobku\Documents\GitHub\Marlin-Bob-2