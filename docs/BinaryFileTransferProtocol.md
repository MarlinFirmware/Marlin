# Marlin Binary File Transfer (BFT)
Marlin is capable of transferring binary data to the internal storage (SD card) via serial when built with `BINARY_FILE_TRANSFER` enabled. The following is a description of the binary protocol that must be used to conduct transfers once the printer is in binary mode after running `M28 B1`.

## Data Endianness
All data structures are **little-endian**! This means that when constructing the packets with multi-byte values, the lower bits are packed first. For example, each packet should start with a 16-bit start token with the value of `0xB5AD`. The data itself should start with a value of `0xAD` followed by `0xB5` etc.

An example Connection SYNC packet, which is only a header and has no payload:
```
 S   S  P P P    H
 t   y  r a a    e
 a   n  o c y    a
 r   c  t k l    d
 t      o e o    e
        c t a    r
        o   d
        l t      C
          y l    S
          p e
          e n
---- -- - - ---- ----
ADB5 00 0 1 0000 0103
```

## Packet Header

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-------------------------------+---------------+-------+-------+
| Start Token (0xB5AD)          | Sync Number   | Prot- | Pack- |
|                               |               | ocol  | et    |
|                               |               | ID    | Type  |
+-------------------------------+---------------+-------+-------+
| Payload Length                | Header Checksum               |
+-------------------------------+-------------------------------+
```

| Field           | Width   | Description |
|-----------------|---------|---|
| Start Token     | 16 bits | Each packet must start with the 16-bit value `0xB5AD`. |
| Sync Number     |  8 bits | Synchronization value, each packet after sync should increment this value by 1. |
| Protocol ID     |  4 bits | Protocol ID. `0` for Connection Control, `1` for Transfer. See Below. |
| Packet Type     |  4 bits | Packet Type ID. Depends on the Protocol ID, see below. |
| Payload Length  | 16 bits | Length of payload data. If this value is greater than 0, a packet payload will follow the header. |
| Header Checksum | 16 bits | 16-bit Fletchers checksum of the header data excluding the Start Token |

## Packet Payload
If the Payload Length field of the header is non-zero, payload data is expected to follow.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-------------------------------+-------------------------------+
| Payload Data                    ...                           |
+-------------------------------+-------------------------------+
| ...                           | Packet Checksum               |
+-------------------------------+-------------------------------+
```

| Field           | Width                  | Description |
|-----------------|------------------------|---|
| Payload Data    | Payload Length bytes   | Payload data. This should be no longer than the buffer length reported by the Connection SYNC operation. |
| Packet Checksum | 16 bits                | 16-bit Fletchers checksum of the header and payload, including the Header Checksum, but excluding the Start Token. |

## Fletchers Checksum
Data packets require a checksum for the header and a checksum for the entire packet if the packet has a payload. In both cases the checksum does not include the first two bytes of the packet, the Start Token.

A simple example implementation:
```c++
uint16_t cs = 0;
for (size_t i = 2; i<packet.size(); i++) {
    uint8_t cslow = (((cs & 0xFF) + packet[i]) % 255);
    cs = ((((cs >> 8) + cslow) % 255) << 8) | cslow;
}
```

## General Responses

### ok
All packets **except** the SYNC Packet (see below) are acknowledged by an `ok<SYNC>` message. This acknowledgement only signifies the client has received the packet and that the header was well formed. An `ok` acknowledgement does not signify successful operation in cases where the client also sends detailed response messages (see details on packet types below). Most notably, with the current implementation the client will still respond `ok` when a client sends multiple packets with the same Sync Number, but will not send the proper response or any errors.

**NOTE**: The `ok` acknowledgement is sent before any packet type specific output. The `SYNC` value should match the Sync Number of the last packet sent, and the next packet sent should use a Sync Number of this value + 1.

Example:
```
ok1
```

### rs
In the case of a packet being sent out of order, where the Sync Number is not the previous Sync Number + 1, an `rs<SYNC>` message will be sent with the last Sync Number received.

Example:
```
rs1
```

## Connection Control (`Protocol ID` 0)
`Protocol ID` 0 packets control the binary connection itself. There are only 2 types:

| Packet Type | Name | Description |
|---|---|---|
| 1 | SYNC | Synchronize host and client and get connection info. |
| 2 | CLOSE | Close the binary connection and switch back to ASCII. |

### SYNC Packet
A SYNC packet should be the first packet sent by a host after enabling binary mode. On success, a sync response will be sent.

**Note**: This is the only packet that is not acknowledged with an `ok` response.

Returns a sync response:
```
ss<SYNC>,<BUFFER_SIZE>,<VERSION_MAJOR>.<VERSION_MINOR>.<VERSION_PATCH>
```

| Value | Description |
|---|---|
| SYNC | The current Sync Number, this should be used in the next packet sent and incremented by 1 for each packet sent after. |
| BUFFER_SIZE | The client buffer size. Packet Payload Length must not exceed this value. |
| VERSION_MAJOR | The major version number of the client Marlin BFT protocol, e.g., `0`. |
| VERSION_MINOR | The minor version number of the client Marlin BFT protocol, e.g., `1`. |
| VERSION_PATCH | The patch version number of the client Marlin BFT protocol, e.g., `0`. |

Example response:
```
ss0,96,0.1.0
```

### CLOSE Packet
A CLOSE packet should be the last packet sent by a host. On success, the client will switch back to ASCII mode.

## Transfer  Control (`Protocol ID` 1)
`Protocol ID` 1 packets control the file transfers performed over the connection:

| Packet Type | Name | Description |
|---|---|---|
| 0 | QUERY | Query the client protocol details and compression parameters. |
| 1 | OPEN  | Open a file for writing and begin accepting data to transfer. |
| 2 | CLOSE | Finish writing and close the current file. |
| 3 | WRITE | Write data to an open file. |
| 4 | ABORT | Abort file transfer. |

### QUERY Packet
A QUERY packet should be the second packet sent by a host, after a SYNC packet. On success a query response will be sent in addition to an `ok<sync>` acknowledgement.

Returns a query response:
```
PFT:version:<VERSION_MAJOR>.<VERSION_MINOR>.<VERSION_PATCH>:compression:<COMPRESSION_ALGO>(,<COMPRESSION_PARAMS>)
```

| Value | Description |
|---|---|
| VERSION_MAJOR | The major version number of the client Marlin BFT protocol, e.g., `0`. |
| VERSION_MINOR | The minor version number of the client Marlin BFT protocol, e.g., `1`. |
| VERSION_PATCH | The patch version number of the client Marlin BFT protocol, e.g., `0`. |
| COMPRESSION_ALGO | Compression algorithm. Currently either `heatshrink` or `none` |
| COMPRESSION_PARAMS | Compression parameters, separated by commas. Currently, if `COMPRESSION_AGLO` is heatshrink, this will be the window size and lookahead size. |

Example response:
```
PFT:version:0.1.0:compression:heatshrink,8,4
```

### OPEN Packet
Opens a file for writing. The filename and other options are specified in the Packet Payload. The filename can be a long filename if the firmware is compiled with support, however the entire Packet Payload must not be longer than the buffer length returned by the SYNC Packet. The filename value must include a null terminator.

Payload:
```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+---------------+---------------+-------------------------------+
| Dummy         | Compression   | Filename                      |
+---------------------------------------------------------------|
| ...                                                           |
+-------------------------------+-------------------------------+
| ...           | NULL (0x0)    | Packet Checksum               |
+-------------------------------+-------------------------------+
```

| Field           | Width                  | Description |
|-----------------|------------------------|---|
| Dummy           |  8 bits | A boolean value indicating if this file transfer should be actually carried out or not. If `1`, the client will respond as if the file is opened and accept data transfer, but no data will be written. |
| Compression     |  8 bits | A boolean value indicating if the data to be transferred will be compressed using the algorithm and parameters returned in the QUERY Packet. |
| Filename        |   ...   | A filename including a null terminator byte. |
| Packet Checksum | 16 bits | 16-bit Fletchers checksum of the header and payload, including the Header Checksum, but excluding the Start Token. |

Responses:

| Response | Description |
|---|---|
| `PFT:success` | File opened and ready for write. |
| `PFT:fail`    | The client couldn't open the file. |
| `PFT:busy`    | The file is already open. |

### CLOSE Packet
Closes the currently open file.

Responses:

| Response | Description |
|---|---|
| `PFT:success` | Buffer flushed and file closed. |
| `PFT:ioerror` | Client storage device failure. |
| `PFT:invalid` | No file open. |

### WRITE Packet
Writes payload data to the currently open file. If the file was opened with Compression set to 1, the data will be decompressed first. Payload Length must not exceed the buffer size returned by the SYNC Packet.

Responses:
On success, an `ok<SYNC>` response will be sent. On error, an `ok<SYNC>` response will be followed by an error response:

| Response | Description |
|---|---|
| `PFT:ioerror` | Client storage device failure. |
| `PFT:invalid` | No file open. |

### ABORT Packet
Closes the currently open file and remove it.

Responses:

| Response | Description |
|---|---|
| `PFT:success` | Transfer aborted, file removed. |

## Typical Usage

1. Send ASCII command `M28 B1` to initiate Binary Transfer mode.
2. Send Connection SYNC Packet, record Sync Number and Buffer Size.
3. Send Transfer QUERY Packet, using Sync Number from above. Record compression algorithm and parameters.
4. Send Transfer OPEN Packet, using last Sync Number + 1, filename and compression options. If error, send Connection CLOSE Packet and abort.
5. Send Transfer Write Packets, using last Sync Number + 1 with the file data. The Payload Length must not exceed the Buffer Size reported in step 2. On error, send a Transfer ABORT Packet, followed by a Connection CLOSE Packet and then abort transfer.
6. Send Transfer CLOSE Packet, using last Sync Number + 1.
7. Send Connection CLOSE Packet, using last Sync Number + 1.
8. Client is now in ASCII mode, transfer complete
