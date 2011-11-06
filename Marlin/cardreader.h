#ifndef __CARDREADERH
#define __CARDREADERH

#ifdef SDSUPPORT
 
#include "SdFat.h"

class CardReader
{
public:
  CardReader();
  
  void initsd();
  void write_command(char *buf);
  //files auto[0-9].g on the sd card are performed in a row
  //this is to delay autostart and hence the initialisaiton of the sd card to some seconds after the normal init, so the device is available quick after a reset

  void checkautostart(bool x); 
  
  void closefile();
  void getfilename(const uint8_t nr);
  uint8_t getnrfilenames();

public:
  bool savetosd;
  SdFile file;
  uint32_t filesize;
  uint32_t sdpos ;
  bool sdmode ;
  SdFile root;
  bool sdactive ;
  char filename[11];
private:
   Sd2Card card;
   SdVolume volume;

   //int16_t n;
   unsigned long autostart_atmillis;

   bool autostart_stilltocheck; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.
};
  
#endif //SDSUPPORT
  
  
  
#endif