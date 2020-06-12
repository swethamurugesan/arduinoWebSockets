/******************************************************************************************************
 * Name: jsonLibrary.h
 * Created: 23/03/2020 11:00:00 AM
 * Version: 1.0
 * Author: Rajat 
 * Description: Library to parse json string.
 *****************************************************************************************************/
  #ifndef jsonLibrary
  #define jsonLibrary
  #include "Arduino.h"
  #include <ArduinoJson.h>
  extern JsonDocument json_parser(char json_message[]);//json parsing function
  #endif
