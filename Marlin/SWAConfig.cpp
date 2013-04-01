/*
 Code to configure and handle the Sanguinololu Wireless Adapter.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 Created on 28 Feb 2013 by ODDepot.com
*/


#include "Marlin.h"
#include "SWAConfig.h"


void setup_SWA(MarlinSerial MYSERIAL) {
  /*
    The WT41 (the bluetooth module on the Sanguinololu Wireless Adapter) enters command mode on
    startup and REMAINS there until a bluetooth connection is established. If any data is sent
    by Marlin, iWRAP (the firmware on the WT41) will assume it's a command and issue a
    'SYNTAX ERROR' if it doesn't recognize it.  If this occurs too many times in a second it will
    overburden the WT41 and the WT41 will not have enough resources to make a proper bluetooth 
    connection. This code is necessary to throttle communication on the serial port and give the
    WT41 the resources it needs to make a connection and enter data mode.
  */

  // Tell the WT41 to turn on the LED, this is a test to make sure the WT41 is responding
  delay(1000);  // The WT41 needs some extra time to process commands
  MYSERIAL.write("PIO SETDIR FF FF\r\n");  // Set PIO direction
  delay(1000);
  MYSERIAL.write("PIO SET FF 00\r\n");  // Set PIO output (turn LED on)

  // Other configurations or commands should be done here, see the WT41 datasheet for details
  // delay(1000);
  // MYSERIAL.write("\r\n")

  // Wait here until a bluetooth connection is established
  while(1){
    MYSERIAL.flush();  // Make sure there's nothing in the receive buffer

    // Send string to the WT41 and wait for a reply (if there is one)
    MYSERIAL.write("WT41 intialized\r\n");
    delay(1000);

    // If the WT41 has entered data mode (it will once a bluetooth connection is established) there
    // won't be a reply and the string just sent should be passed on to the host software
    if(!MYSERIAL.available()){
      break; // Done here, break the loop and exit the function
    }
  }

  // This is suppose to be the preferred method of entering data mode but it doesn't work, maybe
  // it's a bug in iWRAP?
  /*delay(1000);
  MYSERIAL.write("+++");  // Switch from command mode to data mode
  delay(1000);
  MYSERIAL.write("WT41 intialized\r\n");*/

}
