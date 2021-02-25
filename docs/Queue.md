# Marlin's command queue concept

Marlin is a software that process commands encoded in G-Code coming from multiple different source (serial port, SD card, WIFI connection, and other).
It's also processing this command by converting them to actions on many physical actuators (motors, resistances, lasers, RGB leds and other).

Since physical actuators are limited by physic, a synchronization must be achieved between the source of the commands and the physical movement or action.

The synchronization is done via a queue that's buffering/adapting the next commands in memory so that the command processor is never starved (whenever possible).

There are multiple buffers in the system to achieve this goal:
```
+------+                                Marlin's GCodeQueue
|      |                             +--------------------------------------+     +-----------+
| Host |                             |  SerialState         RingBuffer      |     |           |
|      |             Marlin          |  NUM_SERIAL           BUF_SIZE       |     | Marlin    |
+--+---+        R/TX_BUFFER_SIZE     |    +---+        +------------------+ |     |           |
   |             +------------+      |    |   |        |                  | |     | GCode     |
   |             |            |      |    |   |        |   MAX_CMD_SIZE   +-+-----> processor |
   |             | Platform   |      |    |   | On EOL | +--------------+ | r_pos |           |
   +-------------> serial's   +----------->   +--------> |              | | |     +-----------+
                 | buffer     |      |    |   |  w_pos | | CommandQueue | | |
                 |            |      |    |   |        | |              | | |
                 +------------+      |    +---+        | +--------------+ | |
                                     |  Line buffer    |    x BUF_SIZE    | |
                                     |                 |                  | |
                                     |                 |                  | |
                                     |                 |                  | |
                                     |                 |                  | |
                                     |                 +------------------+ |
                                     |                                      |
                                     |                                      |
                                     |                                      |
                                     +--------------------------------------+
```

Marlin is not a multithread or multiprocess software, so the processing loop performs the following actions sequentially:
1. Check if data is available in the platform's serial buffer and in that case, move them into the per-serial line buffer
2. If the data in the line buffer contains a End Of Line character, it's committed to the ring buffer of CommandQueue
3. Loop to 1 until there is no more data in the serial buffer
4. Run the G-Code processor by pop'ing from the CommandQueue ring buffer and execute one command
5. Loop to 1


## Synchronization
To achieve synchronization, Marlin sends a `ok` answer to the host when it's done processing a command (end of G-Code processor task).
Most host will then wait until this answer is received to feed more command into the serial's buffer.


In some case, if no data is available in the serial buffer for a long time, Marlin can ask the host to send more data via the `wait` message.


## Limitation of the design

Please notice few limitations here:
1. While the G-Code processor is busy processing a command, the G-Code queue is not running. 
2. If the host is sending data to the serial buffer, then the serial buffer will fill up (by interrupting the CPU).
3. This means that the serial buffer must be able to contain at least a complete line of data (the size of the serial buffer must be greater than the maximum G-Code command line the host can generate). The default value `MAX_CMD_SIZE` is 96 bytes for Marlin's CommandQueue lines, so a serial buffer that's smaller than 96 bytes can loose data. 
4. Since serial buffer size are likely used as ring buffer themselves, their size should be a power of 2 (64 or 128 bytes recommanded)
5. A host generating too many G-Code command simultaneously will saturate the GCodeQueue but will not improve the processing rate of Marlin since only one command is processed per loop iteration.
6. Because of this, having a large BUF_SIZE is likely useless. The default value of 4 is good for a single serial port. 

