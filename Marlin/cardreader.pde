#ifdef SDSUPPORT
#include "cardreader.h"

CardReader::CardReader()
{
   filesize = 0;
   sdpos = 0;
   sdmode = false;
   sdactive = false;
   savetosd = false;
   autostart_atmillis=0;

   autostart_stilltocheck=true; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.
  //power to SD reader
  #if SDPOWER > -1
    SET_OUTPUT(SDPOWER); 
    WRITE(SDPOWER,HIGH);
  #endif //SDPOWER
  
  autostart_atmillis=millis()+5000;
}

void CardReader::initsd()
{
  sdactive = false;
  #if SDSS >- 1
    if(root.isOpen())
      root.close();
    if (!card.init(SPI_FULL_SPEED,SDSS))
    {
      //if (!card.init(SPI_HALF_SPEED,SDSS))
      SERIAL_ECHOLN("SD init fail");
    }
    else if (!volume.init(&card))
    {
      SERIAL_ERRORLN("volume.init failed");
    }
    else if (!root.openRoot(&volume)) 
    {
      SERIAL_ERRORLN("openRoot failed");
    }
    else 
    {
      sdactive = true;
      SERIAL_ECHOLN("SD card ok");
    }
  #endif //SDSS
}

void CardReader::write_command(char *buf)
{
  char* begin = buf;
  char* npos = 0;
  char* end = buf + strlen(buf) - 1;

  file.writeError = false;
  if((npos = strchr(buf, 'N')) != NULL)
  {
    begin = strchr(npos, ' ') + 1;
    end = strchr(npos, '*') - 1;
  }
  end[1] = '\r';
  end[2] = '\n';
  end[3] = '\0';
  file.write(begin);
  if (file.writeError)
  {
    SERIAL_ERRORLN("error writing to file");
  }
}


void CardReader::checkautostart(bool force)
{
  if(!force)
  {
    if(!autostart_stilltocheck)
      return;
    if(autostart_atmillis<millis())
      return;
  }
  autostart_stilltocheck=false;
  if(!sdactive)
  {
    initsd();
    if(!sdactive) //fail
      return;
  }
  static int lastnr=0;
  char autoname[30];
  sprintf(autoname,"auto%i.g",lastnr);
  for(int i=0;i<(int)strlen(autoname);i++)
    autoname[i]=tolower(autoname[i]);
  dir_t p;

  root.rewind();
  
  bool found=false;
  while (root.readDir(p) > 0) 
  {
    for(int i=0;i<(int)strlen((char*)p.name);i++)
    p.name[i]=tolower(p.name[i]);
    //Serial.print((char*)p.name);
    //Serial.print(" ");
    //Serial.println(autoname);
    if(p.name[9]!='~') //skip safety copies
    if(strncmp((char*)p.name,autoname,5)==0)
    {
      char cmd[30];

      sprintf(cmd,"M23 %s",autoname);
      enquecommand(cmd);
      enquecommand("M24");
      found=true;
    }
  }
  if(!found)
    lastnr=-1;
  else
    lastnr++;
}

void CardReader::closefile()
{
 file.sync();
  file.close();
  savetosd = false; 
}

void CardReader::getfilename(const uint8_t nr)
{

  dir_t p;
  root.rewind();
  uint8_t cnt=0;
  filename[0]='\0';
  while (root.readDir(p) > 0)
  {
    if (p.name[0] == DIR_NAME_FREE) break;
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.'|| p.name[0] == '_') continue;
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
    if(p.name[8]!='G') continue;
    if(p.name[9]=='~') continue;
    if(cnt++!=nr) continue;
    //Serial.println((char*)p.name);
    uint8_t writepos=0;
    for (uint8_t i = 0; i < 11; i++) 
    {
      if (p.name[i] == ' ') continue;
      if (i == 8) {
        filename[writepos++]='.';
      }
      filename[writepos++]=p.name[i];
    }
    filename[writepos++]=0;
  }
}

uint8_t CardReader::getnrfilenames()
{
  dir_t p;
  root.rewind();
  uint8_t cnt=0;
  while (root.readDir(p) > 0)
  {
    if (p.name[0] == DIR_NAME_FREE) break;
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.'|| p.name[0] == '_') continue;
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
    if(p.name[8]!='G') continue;
    if(p.name[9]=='~') continue;
    cnt++;
  }
  return cnt;
}


#endif //SDSUPPORT