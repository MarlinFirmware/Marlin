#include "mks_wifi_gcodes.h"
#include "../../module/settings.h"

#ifdef MKS_WIFI

const uint8_t pak[5]={0xA5,0x07,0x00,0x00,0xFC};

const char m997_idle[]="M997 IDLE\n";
const char m997_printing[]="M997 PRINTING\n";
const char m997_pause[]="M997 PAUSE\n";

const char m115_firmware[]="FIRMWARE_NAME:TFT24\n";

void mks_m991(void){
  char tempBuf[100];
  const int8_t target_extruder = GcodeSuite::get_target_extruder_from_command();
  if (target_extruder < 0) return;

  memset(tempBuf,0,100);

  sprintf((char *)tempBuf,"T:%d /%d B:%d /%d T0:%d /%d T1:0 /0 @:0 B@:0\n", 
  (int)Temperature::degHotend(target_extruder),Temperature::degTargetHotend(target_extruder),
  (int)Temperature::degBed(),Temperature::degTargetBed(),
  (int)Temperature::degHotend(target_extruder),Temperature::degTargetHotend(target_extruder));

  mks_wifi_out_add((uint8_t *)tempBuf,strlen(tempBuf));

  SERIAL_ECHOPGM(STR_OK);
  SERIAL_EOL();
}

void mks_m105(void){
  char tempBuf[100];
  const int8_t target_extruder = GcodeSuite::get_target_extruder_from_command();
  if (target_extruder < 0) return;

  memset(tempBuf,0,100);

  sprintf((char *)tempBuf,"T:%.1f /%.1f B:%.1f /%.1f T0:%.1f /%.1f T1:0.0 /0.0 @:0 B@:0\n",
  Temperature::degHotend(target_extruder),(float)Temperature::degTargetHotend(target_extruder),
  Temperature::degBed(),(float)Temperature::degTargetBed(),
  Temperature::degHotend(target_extruder),(float)Temperature::degTargetHotend(target_extruder));

  mks_wifi_out_add((uint8_t *)tempBuf,strlen(tempBuf));

  SERIAL_ECHOPGM(STR_OK);
  SERIAL_EOL();

}



void mks_m997(void){
  if(CardReader::isPrinting()){
    mks_wifi_out_add((uint8_t *)m997_printing,strlen(m997_printing));
  }else if(CardReader::isPaused()){
    mks_wifi_out_add((uint8_t *)m997_pause,strlen(m997_pause));
  }else{
    mks_wifi_out_add((uint8_t *)m997_idle,strlen(m997_idle));
  }
}


void mks_m115(void){
  char   version[64];
  strcpy(version, "FIRMWARE_NAME: ");
  strcat(version, SHORT_BUILD_VERSION);
  strcat(version, "\n");
  mks_wifi_out_add((uint8_t *)version,strlen(version));
  sprintf(version, "EPROM datasize: %u bytes", settings.datasize());
  strcat(version, "\n");
  mks_wifi_out_add((uint8_t *)version,strlen(version));
}

void mks_m27(void){

  // if (CardReader::isPrinting()) {
  //   SERIAL_ECHOPGM(STR_SD_PRINTING_BYTE);
  //   SERIAL_ECHO(0);
  //   SERIAL_CHAR('/');
  //   SERIAL_ECHOLN(100);
  
  //   SERIAL_ECHOPGM(STR_OK);
  //   SERIAL_EOL();

  // }
}

void mks_m20(bool longnames)
{
  if(IS_SD_PRINTING())
    return;

  if (card.flag.mounted)
  {

    mks_wifi_out_add ((uint8_t*)parser.string_arg, strlen(parser.string_arg));
    mks_wifi_out_add ((uint8_t*)"\n", 1);
    mks_wifi_out_add ((uint8_t*)STR_BEGIN_FILE_LIST, strlen(STR_BEGIN_FILE_LIST));
    mks_wifi_out_add ((uint8_t*)"\n", 1);
    card.ls(TERN_(LONG_FILENAME_HOST_SUPPORT, longnames));
    mks_wifi_out_add ((uint8_t*)STR_END_FILE_LIST, strlen(STR_END_FILE_LIST));
    mks_wifi_out_add ((uint8_t*)"\n", 1);
  }
}

/*
void mks_printListing(SdFile parent
  OPTARG(LONG_FILENAME_HOST_SUPPORT, const bool includeLongNames)
  , const char * const prepend
  OPTARG(LONG_FILENAME_HOST_SUPPORT, const char * const prependLong)
)
{
  dir_t p;

  #if ENABLED(MKS_WIFI)
    serial_index_t port = queue.ring_buffer.command_port();
    char f_name_buf[100];
  #endif

  while (parent.readDir(&p, card.longFilename) > 0) {
    if (DIR_IS_SUBDIR(&p)) {

      size_t lenPrepend = prepend ? strlen(prepend) + 1 : 0;
      // Allocate enough stack space for the full path including / separator
      char path[lenPrepend + FILENAME_LENGTH];
      if (prepend) {
        strcpy(path, prepend);
        path[lenPrepend - 1] = '/';
      }
      char* dosFilename = path + lenPrepend;
      createFilename(dosFilename, p);

      // Get a new directory object using the full path
      // and dive recursively into it.
      SdFile child; // child.close() in destructor
      if (child.open(&parent, dosFilename, O_READ))
      {
        if (includeLongNames) {
          size_t lenPrependLong = prependLong ? strlen(prependLong) + 1 : 0;
          // Allocate enough stack space for the full long path including / separator
          char pathLong[lenPrependLong + strlen(longFilename) + 1];
          if (prependLong) {
            strcpy(pathLong, prependLong);
            pathLong[lenPrependLong - 1] = '/';
          }
          strcpy(pathLong + lenPrependLong, longFilename);
          printListing(child, true, path, pathLong);
        }
      }
      else
      {
        SERIAL_ECHO_MSG(STR_SD_CANT_OPEN_SUBDIR, dosFilename);
        return;
      }
    }
    else if (card.is_dir_or_gcode(p)) {
      if (prepend) {
      #if ENABLED(MKS_WIFI)
        if(port.index != MKS_WIFI_SERIAL_NUM)
        {
          mks_wifi_out_add((uint8_t*)prepend, strlen(prepend));
          mks_wifi_out_add((uint8_t*)"/", 1);
        }
        else
        {
          SERIAL_ECHO(prepend);
          SERIAL_CHAR('/');
        }
      #else
        SERIAL_ECHO(prepend);
        SERIAL_CHAR('/');
      #endif
      }
      #if ENABLED(MKS_WIFI)
        if (port.index == MKS_WIFI_SERIAL_NUM){
          printLongPath(createFilename(filename, p));
        }else{
        SERIAL_ECHO(createFilename(filename, p));
        SERIAL_CHAR(' ');
        SERIAL_ECHOLN(p.fileSize);
      }
      #else
        SERIAL_ECHO(createFilename(filename, p));
        SERIAL_CHAR(' ');
        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          if (!includeLongNames)
        #endif
            SERIAL_ECHOLN(p.fileSize);
        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          else {
            SERIAL_ECHO(p.fileSize);
            SERIAL_CHAR(' ');
            if (prependLong) {
              SERIAL_ECHO(prependLong);
              SERIAL_CHAR('/');
            }
            SERIAL_ECHOLN(longFilename[0] ? longFilename : "???");
          }
        #endif
      #endif  // ENABLED(MKS_WIFI)
    }
  }
}

  void mks_printLongPath(char * const path)
  {
    char f_name_buf[100];
    int i, pathLen = strlen(path);

    // Zero out slashes to make segments
    for (i = 0; i < pathLen; i++) if (path[i] == '/') path[i] = '\0';

    SdFile diveDir = root; // start from the root for segment 1
    for (i = 0; i < pathLen;) {

      if (path[i] == '\0') i++; // move past a single nul

      char *segment = &path[i]; // The segment after most slashes

      // If a segment is empty (extra-slash) then exit
      if (!*segment) break;

      // Go to the next segment
      while (path[++i]) { }

      //SERIAL_ECHOLNPGM("Looking for segment: ", segment);

      // Find the item, setting the long filename
      diveDir.rewind();
        strcpy(f_name_buf,segment);
        card.selectByName(diveDir, f_name_buf);
      #else
        selectByName(diveDir, segment);
      #endif

      // Print /LongNamePart to serial output
      #if ENABLED(MKS_WIFI)
        if(port.index == MKS_WIFI_SERIAL_NUM)
        {
          mks_wifi_out_add((uint8_t*)"/", 1);
        }
        else
          SERIAL_CHAR('/');
      #else
        SERIAL_CHAR('/');
      #endif
      #if ENABLED(MKS_WIFI)
        if(port.index == MKS_WIFI_SERIAL_NUM)
        {
          strcpy(f_name_buf, (longFilename[0] ? longFilename : "???"));
          strcat(f_name_buf, "\n");
          mks_wifi_out_add((uint8_t*)f_name_buf, strlen(f_name_buf));
        }
        else
          SERIAL_ECHO(longFilename[0] ? longFilename : "???");
      #else
        SERIAL_ECHO(longFilename[0] ? longFilename : "???");
      #endif

      // If the filename was printed then that's it
      if (!flag.filenameIsDir) break;

      // SERIAL_ECHOPGM("Opening dir: "); SERIAL_ECHOLN(segment);

      // Open the sub-item as the new dive parent
      SdFile dir;
      if (!dir.open(&diveDir, segment, O_READ)) {
        SERIAL_EOL();
        SERIAL_ECHO_START();
        SERIAL_ECHOPGM(STR_SD_CANT_OPEN_SUBDIR, segment);
        break;
      }

      diveDir.close();
      diveDir = dir;

    } // while i<pathLen

    SERIAL_EOL();
  }
*/

void mks_m30(char *filename){
  
  filename[0]='0';
  sd_delete_file(filename);

  SERIAL_ECHOPGM(STR_OK);
  SERIAL_EOL();

}

void mks_m23(char *filename){
    char dosfilename[14];
    uint8_t dot_pos;

    if(filename[0] == '/'){
      for(uint32_t i = 0; i < strlen(filename) ; i++){
        filename[i] = filename[i+1];
      }
    }

    //Имя файла может быть меньше 12 символов, но с расширением .gcode
    //С конца имени файла шагаем к началу, считаем сколько символов до точки
    dot_pos=0;
    for (char *fn = (filename+strlen(filename) - 1); fn > filename; --fn){
      dot_pos++;  
      if (*fn == '.') break;
    }
   
    if((strlen(filename) > 12) || (dot_pos > 4)){
      if(get_dos_filename(filename,dosfilename)){
        strcpy(CardReader::longFilename,filename); //Для отображения на экране
        card.openFileRead(dosfilename);
    }else{
      ERROR("Can't find dos file name");
    }

    }else{
      card.openFileRead(filename);
    }

  SERIAL_ECHOPGM(STR_OK);
  SERIAL_EOL();

}

#endif
