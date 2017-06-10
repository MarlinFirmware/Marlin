#include "Arduino.h"
#include "Macros.h"
#include "Configuration.h"
#include "serial.h"
#include "temperature.h"

#define BLINK_LED LED_BUILTIN

//wave tables, 4 bit move, +/- 7
uint8_t block_moves[16][8] = {
    { 1, 1, 1, 1, 1, 1, 1, 0 },//14 = 7
    { 1, 1, 1, 0, 1, 1, 1, 0 },//13 = 6
    { 0, 1, 1, 0, 1, 0, 1, 1 },//12 = 5
    { 0, 1, 0, 1, 0, 1, 0, 1 },//11 = 4
    { 0, 1, 0, 0, 1, 0, 0, 1 },//10 = 3
    { 0, 0, 1, 0, 0, 0, 1, 0 },//9 = 2
    { 0, 0, 0, 0, 1, 0, 0, 0 },//8 = 1

    { 0, 0, 0, 0, 0, 0, 0, 0 },//7 = 0

    { 0, 0, 0, 0, 1, 0, 0, 0 },//8 = 1
    { 0, 0, 1, 0, 0, 0, 1, 0 },//9 = 2
    { 0, 1, 0, 0, 1, 0, 0, 1 },//10 = 3
    { 0, 1, 0, 1, 0, 1, 0, 1 },//11 = 4
    { 0, 1, 1, 0, 1, 0, 1, 1 },//12 = 5
    { 1, 1, 1, 0, 1, 1, 1, 0 },//13 = 6
    { 1, 1, 1, 1, 1, 1, 1, 0 },//14 = 7
    { 0 }
};

void send_chunk_ok() {
  static uint8_t chunk_respond_busy_done = 0;

  //concurrently keep up with response counter, should auto roll over
  while(chunk_respond_busy_done != chunk_respond_busy) {
    SERIAL_PROTOCOLPGM("!busy");
    SERIAL_EOL;
    chunk_respond_busy_done++;
  }

  //TODO: this needs to somehow respect ordering. Should we start from the current index?
  for(int n = 0 ; n < NUM_CHUNK_BUFFERS ; n++) {
    const int i = (uint8_t)(chunk_buffer_idx + n) % (NUM_CHUNK_BUFFERS - 1);

    switch(chunk_response[i]) {
    case CHUNK_RESPONSE_NONE:
    case CHUNK_RESPONSE_PENDING:
      break;
    case CHUNK_RESPONSE_OK:
      SERIAL_PROTOCOLPGM("!ok ");
      SERIAL_ECHO(i);
      SERIAL_EOL;

      chunk_response[i] = CHUNK_RESPONSE_PENDING;

      break;
    case CHUNK_RESPONSE_FAIL:
      SERIAL_PROTOCOLPGM("!fail");
      SERIAL_EOL;

      chunk_response[i] = CHUNK_RESPONSE_NONE;

      break;
    }
  }
}


