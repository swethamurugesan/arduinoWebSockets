/******************************************************************************************************
 * Name: jsonLibrary.cpp
 * Created: 23/03/2020 11:00:00 AM
 * Version: 1.0
 * Author: Rajat 
 * Description: Library to parse json string.
 *****************************************************************************************************/

  #include "jsonLibrary.h"
  #include "Arduino.h"
  #include <ArduinoJson.h>
  StaticJsonDocument<100> doc;//StaticJsonDocument<N> allocates memory on the stack
  
/******************************************************************************************
*
*  FUNCTION NAME :  json_parser 
*
* DESCRIPTION   :   To parse incoming JSON string.
*
* PARAMETERS    :   JSON String
*
*******************************************************************************************/
  JsonDocument json_parser(char json_message[])
  {
      DeserializationError error = deserializeJson(doc, json_message);// Deserialize the JSON document
        if (error) 
        {
            Serial.print(F("deserializeJson() failed: "));
             Serial.println(error.c_str());
         }
          return doc;
  }

