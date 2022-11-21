## MKS V1.1 Board

To compile Marlin for this board set `MOTHERBOARD` to `BOARD_MKS_ROBIN_E3_V1_1` (not `BOARD_MKS_ROBIN_E3`).

Changes in MKS Version 1.1:
  - `Z_STEP_PIN` is now `PC14`
  - `Z_DIR_PIN` is now `PC15`
  - The EEPROM on the board is the AT24C32D (32KB)

Full specs and documentation for MKS E3 hardware can be found at https://github.com/makerbase-mks/MKS-Robin-E3-E3D/tree/master/hardware
