import marlin

marlin.relocate_firmware("0x08005000")
marlin.custom_ld_script("ZONESTAR_ZM3E4V2.ld")
