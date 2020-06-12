/******************************************************************************************
 * 
 * Name        : EEPROMlibrary
 * 
 * Version     : 1.1
 * 
 * Author      : Dinesh 
 * 
 * Project     : SWMS-Lite-WiFi
 * 
 * Description : Handles storing and retrieving data to and from controller(Nodemcu-esp8266) memory. (Library file)
 *
 ******************************************************************************************/

/****C LIBRARIES****/
#include <String.h>                                          

/****ARDUINO LIBRARIES****/
#include "Arduino.h"
#include<EEPROM.h> 

/****LOCATION DEFINITIONS****/
#define DEV_ID_LOCATION 0
#define COMMUNICATION_NUMBER 22
#define SECOND_CLIENT_ID_LOCATION 64
#define HOME_LATTITUDE 106
#define HOME_LONGITUDE 128
#define RADIUS 150
#define TIMER_DATA_LOCATION 172
#define APN 182
#define REPEAT_LOCATION 185
#define ACTUATOR_STATUS_LOCATION 189

/****SIZE DEFINITIONS****/
#define MAX_LENGTH_OF_DEVICEID 20
#define MAX_LENGTH_OF_FIRST_CLIENT_ID 40
#define MAX_LENGTH_OF_SECOND_CLIENT_ID 40
#define MAX_LENGTH_OF_IP_ADDRESS 20
#define MAX_LENGTH_OF_WIFI_SSID 20
#define MAX_LENGTH_OF_WIFI_PASSWORD 20
#define MAX_LENGTH_OF_TIMER_DATA 8
#define MAX_LENGTH_OF_TEMP_REPEAT_DATA 1
#define MAX_LENGTH_OF_REPEAT_DATA 1
#define MAX_LENGTH_OF_ACTUATOR_STATUS 1

/****EXTERNAL LIBRARIES****/
#include "eeprom_library.h"

/****EXTERNAL VARIABLES****/
String OutData="";                                          //assign check--assign in init function

/***INTERNAL FUNCTIONS****/
String ReadFromEeprom(char add);
void WriteToEeprom(char add,String data);

/***EXTERNAL FUNCTIONS****/
int StoreInfo(String InIdentifier,String InData);            //if data is null check -- check all incoming parameters
String RetrieveInfo(String OutIdentifier);
int ResetRepeatData();

/****************************
Function Name :ReadFromEeprom

Description   :Function to read strings from eeprom

Return Data   :Returns requested data if available in Eeprom
****************************/
String ReadFromEeprom(char add)
{
  int i;
  char data[100]; 
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)                                             //Reads requested data for every bit 
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

/****************************
Function Name :WriteToEeprom

Description   :Function to write strings to eeprom

Return Data   :Writes given data into eeprom
****************************/
void WriteToEeprom(char add,String data)
{                                                          //check if data is null
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)                                                    //Writes given info per bit of storage
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   
  EEPROM.commit();                                                        //Fixes changes done in eeprom
}

/****************************
Function Name :StoreInfo

Description   :Stores given data into eeprom at specified locations                //add parameters with what should be sent

Return Data   :Returns 1 for success, 0 for error in identifiers, -1 for error in length of data
****************************/
int StoreInfo(String InIdentifier,String InData)                        // add null check
{
  int len=InData.length();                                              //fullname of the variable
  
  if(InIdentifier=="DevId")                                               //Storing device id in eeprom   //move around to fix assignment issue!!
  {
    if(len<=10)                                              //#define needed for all ex.MAX_LENGTH_OF_DEVICEID=
    {
      WriteToEeprom(DEV_ID_LOCATION,InData);                                        //declare response to handle from fn
      return 1;
    }
    else
    {
      EEPROM.write(DEV_ID_LOCATION,'\0');                                        //make function and add parameters--call once only
      return -1;                                                    // check eeprom write error return
    }
  }
  
  else if(InIdentifier=="communication_number")                                  //Storing First client id in eeprom
  {
    if(len<=40)
    {
      WriteToEeprom(COMMUNICATION_NUMBER,InData);                                            //another fn that checks length and to eeprom...remove if..elseif.... reduce code size
      return 1;
    }
    else
    {
      EEPROM.write(COMMUNICATION_NUMBER,'\0'); 
      return -1;
    }   
  }
  
  else if(InIdentifier=="SecondClientId")                                 //Storing Second client id in eeprom
  {
    if(len<=40)
    {
      WriteToEeprom(SECOND_CLIENT_ID_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(SECOND_CLIENT_ID_LOCATION,'\0'); 
      return -1;
    }
  }
  
  else if(InIdentifier=="home_lattitude")                                      //Storing Ip address in eeprom
  {
    if(len<=20)
    {
      WriteToEeprom(HOME_LATTITUDE,InData);
      return 1;
    }
    else
    {
      EEPROM.write(HOME_LATTITUDE,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="home_longitude")                                       //Storing wifi ssid in eeprom
  {
    if(len<=20)
    {
      WriteToEeprom(HOME_LONGITUDE,InData);
      return 1;
    }
    else
    {
      EEPROM.write(HOME_LONGITUDE,'\0'); 
      return -1;
    }
  }

  else if(InIdentifier=="radius")                                   //Storing wifi password in eeprom
  {
    if(len<=20)
    {
      WriteToEeprom(RADIUS,InData);
      return 1;
    }
    else
    {
      EEPROM.write(RADIUS,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="timer_data")                                      //Storing timer data in eeprom
  {
    if(len<=8)
    {
      WriteToEeprom(TIMER_DATA_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(TIMER_DATA_LOCATION,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="RepeatData")                                     //Storing repeat data in eeprom
  {
    if(len<=1)
    {
      WriteToEeprom(REPEAT_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(REPEAT_LOCATION,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="apn")                                 //Storing repeat data in eeprom
  {
    if(len<=1)
    {
      WriteToEeprom(APN,InData);
      return 1;
    }
    else
    {
      EEPROM.write(APN,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="actuator_status")                                 //Storing Actuator status in eeprom
  {
    if(len<=1)
    {
      WriteToEeprom(ACTUATOR_STATUS_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(ACTUATOR_STATUS_LOCATION,'\0'); 
      return -1;
    }
  }
  else                                                                    //Error handling for storing eeprom data
  {
    return 0;
  }
}

/****************************
Function Name :RetrieveInfo

Description   :Retrieves requested information from eeprom

Return Data   :Returns requested data for success, "error" for error
****************************/
String RetrieveInfo(String OutIdentifier)
{
  if(OutIdentifier=="DevId")                                              //Retrieving device id from eeprom
  {
    OutData=ReadFromEeprom(DEV_ID_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="communication_number")                                 //Retrieving frist client id from eeprom
  {
    OutData=ReadFromEeprom(COMMUNICATION_NUMBER);
    return OutData;
  }
  else if(OutIdentifier=="SecondClientId")                                //Retrieving second client id from eeprom
  {
    OutData=ReadFromEeprom(SECOND_CLIENT_ID_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="home_lattitude")                                     //Retrieving ip address from eeprom
  {
    OutData=ReadFromEeprom(HOME_LATTITUDE);
    return OutData;
  }
  else if(OutIdentifier=="home_longitude")                                      //Retrieving wifi ssid from eeprom
  {
    OutData=ReadFromEeprom(HOME_LONGITUDE);
    return OutData;
  }
  else if(OutIdentifier=="radius")                                  //Retrieving wifi password from eeprom
  {
    OutData=ReadFromEeprom(RADIUS);
    return OutData;
  }
  else if(OutIdentifier=="timer_data")                                     //Retrieving timer data from eeprom
  {
    OutData=ReadFromEeprom(TIMER_DATA_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="RepeatData")                                    //Retrieving repeat data from eeprom
  {
    OutData=ReadFromEeprom(REPEAT_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="apn")                                //Retrieving temporary repeat data from eeprom
  {
    OutData=ReadFromEeprom(APN);
    return OutData;
  }
  else if(OutIdentifier=="actuator_status")                                //Retrieving actuator status from eeprom
  {
    OutData=ReadFromEeprom(ACTUATOR_STATUS_LOCATION);
    return OutData;
  }
  else                                                                    //Error handling for retrieving eeprom data
  {
    return String("error");
  }
}

/****************************
Function Name :ResetRepeatData

Description   :Resets repeat data in eeprom

Return Data   :Returns 1 on success
****************************/
int ResetRepeatData()
{
  WriteToEeprom(172,"1");                                                 //Resets repeat info in eeprom
  return 1;
}
