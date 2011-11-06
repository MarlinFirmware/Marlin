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
  void release();
  void startFileprint();
  void startFilewrite(char *name);
  void pauseSDPrint();
  void getStatus();
  
  void selectFile(char* name);
  void getfilename(const uint8_t nr);
  uint8_t getnrfilenames();
  

  inline void ls() {root.ls();};
  inline bool eof() { sdpos = file.curPosition();return sdpos>=filesize ;};
  inline char get() {  int16_t n = file.read(); return (n!=-1)?(char)n:'\n';};
  inline void setIndex(long index) {sdpos = index;file.seekSet(index);};

public:
  bool saving;
  bool sdprinting ;  
  bool cardOK ;
  char filename[11];
private:
  SdFile root;
  Sd2Card card;
  SdVolume volume;
  SdFile file;
  uint32_t filesize;
  //int16_t n;
  unsigned long autostart_atmillis;
  uint32_t sdpos ;

  bool autostart_stilltocheck; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.
};
  
#endif //SDSUPPORT
  
  
  
#endif