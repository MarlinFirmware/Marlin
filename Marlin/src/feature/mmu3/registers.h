#pragma once
#include <stdint.h>

namespace MMU2 {

// Register map for MMU
enum class Register : uint8_t {
    Project_Major = 0x00,
    Project_Minor = 0x01,
    Project_Revision = 0x02,
    Project_Build_Number = 0x03,
    MMU_Errors = 0x04,
    Current_Progress_Code = 0x05,
    Current_Error_Code = 0x06,
    Filament_State = 0x07,
    FINDA_State = 0x08,
    FSensor_State = 0x09,
    Motor_Mode = 0x0A,
    Extra_Load_Distance = 0x0B,
    FSensor_Unload_Check_Distance = 0xC,
    Pulley_Unload_Feedrate = 0x0D,
    Pulley_Acceleration = 0x0E,
    Selector_Acceleration = 0x0F,
    Idler_Acceleration = 0x10,
    Pulley_Load_Feedrate = 0x11,
    Selector_Nominal_Feedrate = 0x12,
    Idler_Nominal_Feedrate = 0x13,
    Pulley_Slow_Feedrate = 0x14,
    Selector_Homing_Feedrate = 0x15,
    Idler_Homing_Feedrate = 0x16,
    Pulley_sg_thrs_R = 0x17,
    Selector_sg_thrs_R = 0x18,
    Idler_sg_thrs_R = 0x19,
    Get_Pulley_Position = 0x1A,
    Set_Get_Selector_Slot = 0x1B,
    Set_Get_Idler_Slot = 0x1C,
    Set_Get_Selector_Cut_iRun = 0x1D,
    Set_Get_Pulley_iRun = 0x1E,
    Set_Get_Selector_iRun = 0x1F,
    Set_Get_Idler_iRun = 0x20,
    Reserved = 0x21,
};

} // namespace MMU2
