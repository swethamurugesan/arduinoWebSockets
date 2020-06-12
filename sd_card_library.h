/**************************************************************************************
 * 
 * Name          : bluetooth.h
 * 
 * Version       : 1
 * 
 * Author        : Raji
 * 
 * Description   : This library is to connect bluetooth module  with nodemcu12e
 *                  in order to connect to wifi and ip address by sending wifi 
 *                  credentials via bluetooth of SWMS             
 * 
 * Pin Discription: (bluetooth->nodemcu12E) 
 *                           Rx->Tx  
 *                           Tx->Rx
 *                           Vcc->Vcc
 *                           Gnd->Gnd.          
 **************************************************************************************/
  #ifndef SD_CARD_LIBRARY_H_
  #define SD_CARD_LIBRARY_H_
/*************C LIBRARIES********/
  #include <String.h>
  #include "Arduino.h"
  #include "FS.h"
  #include "SD.h"
  #include<Wire.h>
/******declarations************/

  extern String dataMessage; 

/*********************function declarartions****************************/
  extern int sd_initialization(void);
extern void writeFile(fs::FS &fs, const char * path, const char * message);
extern void appendFile(fs::FS &fs, const char * path, const char * message);
extern void readFile(fs::FS &fs, const char * path);
extern void deleteFile(fs::FS &fs, const char * path);
 extern void logSDCard(void);
 
#endif
