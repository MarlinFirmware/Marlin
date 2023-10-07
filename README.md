VERY EXPERIMENTAL BUILD THAT USES DMA CIRCULAR READING TO THE RX SERIAL BUFFER, NO ISR NEEDED.

The Dma Controller Just Does Its Work In The Background And Keeps Copying The Received Serial Characters To The Rx Buffer.
Marlin Will Read And Process The Received Data, Freeing Up The Buffer.
Emergency Parser Is Working Too, But A Bit Different Than Before.

By Default Marlin Uses Interrupt Based Serial Reading Provided By The Arduino Platform.
On My Setup This Causes Data Corruption (Lost Bytes) On My Setup Starting At 250k Baud When Stressing The System.
In Many Cases Data Corruption Can Go Unnoticed, Because The Resulting Gcode Command Is Still Valid.
I Only Found This Data Corruption Issue When I Started Using Crcs For The Gcodes.
I Believe This Is An Issue In The Arduino Platform.

With Dma Serial Reading I Can Use 1m Baud Without Any Issues.
Dma Is Of Course Hardware Dependent. I Only Implemented Stm32f2/F4. 
Test Hardware Used: Mks Monster 8 V1.0 With Btt Tft35v3.0 (Ahead Of Current Build).
