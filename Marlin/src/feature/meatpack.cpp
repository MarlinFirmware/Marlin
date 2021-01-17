/*
* MeatPack G-Code Compression
*
* Algorithm & Implementation: Scott Mudge - mail@scottmudge.com
* Date: Dec. 2020
*/

#include "../inc/MarlinConfig.h"

#if ENABLED(MEATPACK)

#include "meatpack.h"

//#include "language.h"
//#include "Marlin.h"

//#define MP_DEBUG

// Utility definitions
#define MeatPack_CommandByte		0b11111111
#define MeatPack_NextPackedFirst	0b00000001
#define MeatPack_NextPackedSecond	0b00000010

#define MeatPack_SpaceCharIdx       11U
#define MeatPack_SpaceCharReplace   'E'

/*
    Character Frequencies from ~30 MB of comment-stripped gcode:
     '1' -> 4451136
     '0' -> 4253577
     ' ' -> 3053297
     '.' -> 3035310
     '2' -> 1523296
     '8' -> 1366812
     '4' -> 1353273
     '9' -> 1352147
     '3' -> 1262929
     '5' -> 1189871
     '6' -> 1127900
     '7' -> 1112908
    '\n' -> 1087683
     'G' -> 1075806
     'X' ->  975742
     'E' ->  965275
     'Y' ->  965274
     'F' ->   99416
     '-' ->   90242
     'Z' ->   34109
     'M' ->   11879
     'S' ->    9910
     If spaces are omitted, we add 'E'
*/

// Note:
// I've tried both a switch/case method and a lookup table. The disassembly is exactly the same after compilation, byte-to-byte.
// Thus, performance is identical.
#define USE_LOOKUP_TABLE

#ifdef USE_LOOKUP_TABLE
// The 15 most-common characters used in G-code, ~90-95% of all g-code uses these characters
// NOT storing this with PROGMEM, given how frequently this table will be accessed.
uint8_t MeatPackLookupTbl[16] = {
    '0',	// 0000
    '1',	// 0001
    '2',	// 0010
    '3',	// 0011
    '4',	// 0100
    '5',	// 0101
    '6',	// 0110
    '7',	// 0111
    '8',	// 1000
    '9',	// 1001
    '.',	// 1010
    ' ',	// 1011
    '\n',	// 1100
    'G',	// 1101
    'X',	// 1110
    '\0' // never used, 0b1111 is used to indicate next 8-bits is a full character
};
#else
inline uint8_t get_char(register uint8_t in) {
    switch (in) {
    case 0b0000:
        return '0';
        break;
    case 0b0001:
        return '1';
        break;
    case 0b0010:
        return '2';
        break;
    case 0b0011:
        return '3';
        break;
    case 0b0100:
        return '4';
        break;
    case 0b0101:
        return '5';
        break;
    case 0b0110:
        return '6';
        break;
    case 0b0111:
        return '7';
        break;
    case 0b1000:
        return '8';
        break;
    case 0b1001:
        return '9';
        break;
    case 0b1010:
        return '.';
        break;
    case 0b1011:
        return ' ';
        break;
    case 0b1100:
        return '\n';
        break;
    case 0b1101:
        return 'G';
        break;
    case 0b1110:
        return 'X';
        break;
    }
    return 0;
}
#endif


// State variables
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
enum MeatPack_ConfigStateFlags {
    MPConfig_None = 0,
    MPConfig_Active = (1 << 0),
    MPConfig_NoSpaces = (1 << 1)
};

uint8_t mp_config = MPConfig_None; // Configuration state
uint8_t mp_cmd_active = 0;         // Is a command is pending
uint8_t mp_char_buf = 0;           // Buffers a character if dealing with out-of-sequence pairs
uint8_t mp_cmd_count = 0;          // Counts how many command bytes are received (need 2)
uint8_t mp_full_char_queue = 0;    // Counts how many full-width characters are to be received
uint8_t mp_char_out_buf[2];        // Output buffer for caching up to 2 characters
uint8_t mp_char_out_count = 0;     // Stores number of characters to be read out.

// #DEBUGGING
#ifdef MP_DEBUG
uint32_t mp_chars_decoded = 0;
#endif

void FORCE_INLINE mp_handle_output_char(const uint8_t c) {
    mp_char_out_buf[mp_char_out_count++] = c;

#ifdef MP_DEBUG
    if (mp_chars_decoded < 64) {
        ++mp_chars_decoded;
        SERIAL_ECHOPGM("Rec Byte: ");
        MYSERIAL.print("0x");
        MYSERIAL.print((uint8_t)c, HEX);
        SERIAL_ECHOLNPGM("");
    }
#endif
}

// Storing
// packed = ((low & 0xF) << 4) | (high & 0xF);

// Unpacking
// low = (packed >> 4) & 0xF;
// high = (packed & 0xF);

//==========================================================================
uint8_t FORCE_INLINE mp_unpack_chars(const uint8_t pk, uint8_t* __restrict const chars_out) {
    register uint8_t out = 0;

#ifdef USE_LOOKUP_TABLE
    // If lower 4 bytes is 0b1111, the higher 4 are unused, and next char is full.
    if ((pk & MeatPack_FirstNotPacked) == MeatPack_FirstNotPacked) out |= MeatPack_NextPackedFirst;
    else chars_out[0] = MeatPackLookupTbl[(pk & 0xF)]; // Assign lower char

    // Check if upper 4 bytes is 0b1111... if so, we don't need the second char.
    if ((pk & MeatPack_SecondNotPacked) == MeatPack_SecondNotPacked) out |= MeatPack_NextPackedSecond;
    else chars_out[1] = MeatPackLookupTbl[((pk >> 4) & 0xf)]; // Assign upper char
#else
    // If lower 4 bytes is 0b1111, the higher 4 are unused, and next char is full.
    if ((pk & MeatPack_FirstNotPacked) == MeatPack_FirstNotPacked) out |= MeatPack_NextPackedFirst;
    else chars_out[0] = get_char(pk & 0xF); // Assign lower char

    // Check if upper 4 bytes is 0b1111... if so, we don't need the second char.
    if ((pk & MeatPack_SecondNotPacked) == MeatPack_SecondNotPacked) out |= MeatPack_NextPackedSecond;
    else chars_out[1] = get_char((pk >> 4) & 0xf); // Assign upper char
#endif

    return out;
}

//==============================================================================
void FORCE_INLINE mp_reset_state() {
    SERIAL_ECHOLNPGM("MP Reset");
    mp_char_out_count = 0;
    mp_cmd_active = MPCommand_None;
    mp_config = MPConfig_None;
    mp_char_buf = 0;
    mp_cmd_count = 0;
    mp_cmd_active = 0;
    mp_full_char_queue = 0;

#ifdef MP_DEBUG
    mp_chars_decoded = 0;
#endif
}

//==========================================================================
void FORCE_INLINE mp_handle_rx_char_inner(const uint8_t c) {

    // Packing enabled, handle character and re-arrange them appropriately.
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (mp_config & MPConfig_Active) {
        if (mp_full_char_queue > 0) {
            mp_handle_output_char(c);
                if (mp_char_buf > 0) {
                    mp_handle_output_char(mp_char_buf);
                        mp_char_buf = 0;
                }
            --mp_full_char_queue;
        }
        else {
            uint8_t buf[2] = { 0,0 };
            register const uint8_t res = mp_unpack_chars(c, buf);

            if (res & MeatPack_NextPackedFirst) {
                ++mp_full_char_queue;
                if (res & MeatPack_NextPackedSecond) ++mp_full_char_queue;
                else mp_char_buf = buf[1];
            }
            else {
                mp_handle_output_char(buf[0]);
                if (res & MeatPack_NextPackedSecond) ++mp_full_char_queue;
                else mp_handle_output_char(buf[1]);
            }
        }
    }
    else // Packing not enabled, just copy character to output
        mp_handle_output_char(c);
}

//==========================================================================
void FORCE_INLINE mp_echo_config_state() {
    SERIAL_ECHOPGM("[MP] ");

    // Echo current state
    if (mp_config & MPConfig_Active)
        SERIAL_ECHOPGM("ON");
    else
        SERIAL_ECHOPGM("OFF");

    if (mp_config & MPConfig_NoSpaces)
        SERIAL_ECHOPGM("NSP"); // [N]o [SP]aces
    else
        SERIAL_ECHOPGM("ESP"); // [E]nabled [SP]aces

    SERIAL_ECHOLNPGM("");

    // Validate config vars
    MeatPackLookupTbl[MeatPack_SpaceCharIdx] = ((mp_config & MPConfig_NoSpaces) ? MeatPack_SpaceCharReplace : ' ');
}

//==========================================================================
void FORCE_INLINE mp_handle_cmd(const MeatPack_Command c) {
    switch (c) {
    case MPCommand_EnablePacking: {
        mp_config |= MPConfig_Active;
#ifdef MP_DEBUG
        SERIAL_ECHOLNPGM("[MPDBG] ENABL REC");
#endif
    } break;
    case MPCommand_DisablePacking: {
        mp_config &= ~(MPConfig_Active);
#ifdef MP_DEBUG
        SERIAL_ECHOLNPGM("[MPDBG] DISBL REC");
#endif
    } break;
    case MPCommand_TogglePacking: {
        mp_config ^= MPConfig_Active;
#ifdef MP_DEBUG
        SERIAL_ECHOLNPGM("[MPDBG] TGL REC");
#endif
    } break;
    case MPCommand_ResetAll: {
        mp_reset_state();
#ifdef MP_DEBUG
        SERIAL_ECHOLNPGM("[MPDBG] RESET REC");
#endif
    } break;
    case MPCommand_EnableNoSpaces: {
        mp_config |= MPConfig_NoSpaces;
#ifdef MP_DEBUG
        SERIAL_ECHOLNPGM("[MPDBG] ENABL NSP");
#endif
    } break;
    case MPCommand_DisableNoSpaces: {
        mp_config &= ~(MPConfig_NoSpaces);
#ifdef MP_DEBUG
        SERIAL_ECHOLNPGM("[MPDBG] DISBL NSP");
#endif
    } break;
    default: {
#ifdef MP_DEBUG
        SERIAL_ECHOLN("[MPDBG] UNK CMD REC");
#endif
    }
    case MPCommand_QueryConfig:
        break;
    }

    mp_echo_config_state();
}

//==========================================================================
void mp_handle_rx_char(const uint8_t c) {

    // Check for commit complete
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if (c == (uint8_t)(MeatPack_CommandByte)) {
        if (mp_cmd_count > 0) {
            mp_cmd_active = 1;
            mp_cmd_count = 0;
        }
        else
            ++mp_cmd_count;
        return;
    }

    if (mp_cmd_active > 0) {
        mp_handle_cmd((MeatPack_Command)c);
        mp_cmd_active = 0;
        return;
    }

    if (mp_cmd_count > 0) {
        mp_handle_rx_char_inner((uint8_t)(MeatPack_CommandByte));
        mp_cmd_count = 0;
    }

    mp_handle_rx_char_inner(c);
}

//==========================================================================
uint8_t mp_get_result_char(char* const __restrict out) {
    if (mp_char_out_count > 0) {
        const uint8_t res = mp_char_out_count;
        for (register uint8_t i = 0; i < mp_char_out_count; ++i)
            out[i] = (char)mp_char_out_buf[i];
        mp_char_out_count = 0;
        return res;
    }
    return 0;
}

//==============================================================================
void mp_trigger_cmd(const MeatPack_Command cmd)
{
    mp_handle_cmd(cmd);
}

#endif // MEATPACK
