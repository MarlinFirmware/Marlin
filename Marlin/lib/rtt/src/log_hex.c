#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "log.h"


uint8_t log_hex_buffer[1024];
//dump hex str to buffer.

void* log_hex_dump_line(uint8_t data[], uint8_t len, uint8_t buffer[])
{
    for (int i=0; i < len; ++i) {
        sprintf(buffer + i*3, "%02x ", data[i]);
    }
    buffer[len * 3] = '\n';
    // LOGI("%s", buffer);
    return buffer;
}

void* log_hex_dump2(uint8_t data[], uint8_t len, uint8_t line_cell, uint8_t buffer[])
{
    uint16_t line_whole = len /  line_cell;
    uint8_t line_final = len % line_cell;
    uint8_t line_full_len = line_cell * 3 + 1;//include \n
    // uint8_t line_buffer[128];
    for (int i=0; i<line_whole; ++i) {
        log_hex_dump_line(data + line_cell * i, 
            line_cell, 
            buffer + i * line_full_len);
    }
    log_hex_dump_line(data + line_cell * line_whole, 
        line_final, 
        buffer + line_whole*line_full_len);
    return buffer;
}

void* log_hex_dump(uint8_t data[], uint8_t len, uint8_t buffer[])
{
    const uint8_t line_cell = 8;
    uint8_t line_cnt = len / line_cell;
    if (len % line_cell) {
        line_cnt += 1;
    }
    uint32_t idx = 0;
    //each byte we need at least 3 byte to store "2x "
    for (int line=0; line < line_cnt; ++line) {
        for (int i=0; i < line_cell; ++i) {
            sprintf(buffer + 
                    line * (line_cell*3+1) +
                    i*3, 
                    "%02x ", 
                    data[line*line_cell + i]);
        }
        idx += line_cell*3;
        buffer[idx] = '\n';
        // buffer[(line + 1) * (line_cell*3 + 1)] = '\n';
        // sprintf(buffer + (line + 1) * line_cell*3, "%c", "\n");
    }
    return buffer;
}

void log_hex(char const* head, uint8_t data[], uint8_t len)
{
    LOGI("%s%s", head, log_hex_dump(data, len, log_hex_buffer));
}