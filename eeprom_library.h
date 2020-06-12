  
  #ifndef eeprom_library
  #define eeprom_library
/****C LIBRARIES****/
#include <String.h>
#include<EEPROM.h>

/****ARDUINO LIBRARIES****/
#include "Arduino.h"

/****EXTERNAL FUNCTIONS****/
extern int StoreInfo(String InIdentifier,String InData);     //Function for storing data in EEPROM
extern String RetrieveInfo(String OutIdentifier);            //Function for retrieving data from EEPROM      // add description
extern int ResetRepeatData();                                //Function to reset repeat data in EEPROM
extern int clearEeprom(char add);
extern void Write_To_Eeprom(char add,String data);
extern String ReadFromEeprom(char add);
/****EXTERNAL VARIABLES****/
extern String OutData;  
extern int eepromLocation[7];         
#endif

