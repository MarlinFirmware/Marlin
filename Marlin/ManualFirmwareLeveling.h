#ifndef MANUALFIRMWARELEVELING_H
#define MANUALFIRMWARELEVELING_H

void g31_manual_firmware_leveling();
void g32_clear_manual_firmware_leveling();
void set_bed_level_equation(float z_at_xLeft_yFront, float z_at_xRight_yFront, float z_at_xLeft_yBack);

#endif
