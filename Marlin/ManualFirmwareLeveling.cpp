#include "stepper.h"
#include "planner.h"
#include "Marlin.h"
#include "ManualFirmwareLeveling.h"
#include "gcode.h"

#ifdef ENABLE_AUTO_BED_LEVELING

void g31_manual_firmware_leveling()
{
	float z_origin, z_right_front, z_left_back;
	bool ok = true;
	if (code_seen('O')) {
		z_origin = code_value();
	} else {
		ok = false;
	}

	if (code_seen('R')) {
		z_right_front = code_value();
	} else {
		ok = false;
	}

	if (code_seen('B')) {
		z_left_back = code_value();
	} else {
		ok = false;
	}
	
	if (ok) {
		set_bed_level_equation(z_origin, z_right_front, z_left_back);
	} else {
		SERIAL_PROTOCOLPGM("G31 needs O(rigin) R(ight) and B(back) offsets");
	}
}

void g32_clear_manual_firmware_leveling()
{
	st_synchronize();
	plan_bed_level_matrix.set_to_identity();
	SERIAL_PROTOCOLPGM("Bed leveling matrix cleared");
}

#endif /* ENABLE_AUTO_BED_LEVELING */
/* vim: set ts=4 sw=4 autoindent nocindent:  */
