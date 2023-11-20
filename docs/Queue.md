# Marlin's command queue concept

Marlin Firmware processes G-code commands as they arrive from multiple sources, including the SD card and one or more serial ports such as USB-connected hosts, WiFi, Bluetooth, and so on.

Marlin is also continuously processing the commands at the front of the queue, converting them into signals for many physical actuators such as motors, heaters, lasers, and RGB LEDs.

The firmware needs to maintain continuity and timing so the command senders remain unblocked, while still performing physical movements and other actions in real-time, respecting the physical limits of stepper motors and other peripherals.

To keep things flowing Marlin feeds a single queue of G-code commands from all inputs, inserting them in the order received. Movement commands immediately go into the Planner Buffer, if there is room. The buffering of a move is considered the completion of the command, so if a non-movement command has to occur after a move is done, and not just after a move is buffered, then there has to be an `M400` to wait for the Planner Buffer to finish.

Whenever the command queue gets full the sender needs to wait for space to open up, and the host may need to re-send the last command again. Marlin does some handshaking to keep the host informed during a print job, described below.

An opposite problem called "planner starvation" occurs when Marlin receives many short and fast moves in a row so the Planner Buffer gets completed very quickly. In this case the host can't send commands fast enough to prevent the Planner Buffer from emptying out. Planner starvation causes obvious stuttering and is commonly seen on overloaded deltabots during small curves. Marlin has strategies to mitigate this issue, but sometimes a model has to be re-sliced (or the G-code has to be post-processed with Arc Welder) just to stay within the machine's inherent limits.

Here's a basic flowchart of Marlin command processing:
```
+------+                                Marlin's GCodeQueue
|      |                             +--------------------------------------+     +-----------+
| Host |                             |  SerialState         RingBuffer      |     |           |
|      |             Marlin          |  NUM_SERIAL           BUF_SIZE       |     | Marlin    |
+--+---+        R/TX_BUFFER_SIZE     |    +---+        +------------------+ |     |           |
   |             +------------+      |    |   |        |                  | |     | G-Code    |
   |             |            |      |    |   |        |   MAX_CMD_SIZE   +-+-----> processor |
   |             | Platform   |      |    |   | On EOL | +--------------+ | r_pos |           |
   +-------------> serial's   +----------->   +--------> |   G-code     | | |     +-----------+
                 | buffer     |      |    |   |  w_pos | |   command    | | |
                 |            |      |    |   |        | |    line      | | |
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

Marlin is a single-threaded application with a main `loop()` that manages the command queue and an `idle()` routine that manages the hardware. The command queue is handled in two stages:
1. The `idle()` routine reads all inputs and attempts to enqueue any completed command lines.
2. The main `loop()` gets the command at the front the G-code queue (if any) and runs it. Each G-code command blocks the main loop, preventing the queue from advancing until it returns. To keep essential tasks and the UI running, any commands that run a long process need to call `idle()` frequently.

## Synchronization

To maintain synchronization Marlin replies "`ok`" to the host as soon as the command has been enqueued. This lets the host know that it can send another command, and well-behaved hosts will wait for this message. With `ADVANCED_OK` enabled the `ok` message includes extra information (such as the number of slots left in the queue).

If no data is available on the serial buffer, Marlin can be configured to periodically send a "`wait`" message to the host. This was the only method of "host keepalive" provided in Marlin 1.0, but today the better options are `HOST_KEEPALIVE` and `ADVANCED_OK`.

## Limitation of the design

Some limitations to the design are evident:
1. Whenever the G-code processor is busy processing a command, the G-code queue cannot advance.
2. A long command like `G29` causes commands to pile up and to fill the queue, making the host wait.
3. Each serial input requires a buffer large enough for a complete G-code line. This is set by `MAX_CMD_SIZE` with a default value of 96.
4. Since serial buffer sizes are likely used as ring buffers themselves, as an optimization their sizes must be a power of 2 (64 or 128 bytes recommended).
5. If a host sends too much G-code at once it can saturate the `GCodeQueue`. This doesn't do anything to improve the processing rate of Marlin since only one command can be dispatched per loop iteration.
6. With the previous point in mind, it's clear that the longstanding wisdom that you don't need a large `BUF_SIZE` is not just apocryphal. The default value of 4 is typically just fine for a single serial port. (And, if you decide to send a `G25` to pause the machine, the wait will be much shorter!)
