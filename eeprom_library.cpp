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
#define FIRST_CLIENT_ID_LOCATION 22
#define SECOND_CLIENT_ID_LOCATION 64
#define IP_LOCATION 65
#define SSID_LOCATION 68
#define PASSWORD_LOCATION 20
#define TIMER_DATA_LOCATION 32
#define LOWER_THRESHOLD 42
#define REPEAT_LOCATION 55
#define UPPER_THRESHOLD 69

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
int eepromLocation[7]={136,147,158,169,180,242,253};
/***INTERNAL FUNCTIONS****/


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
Function Name :Write_To_Eeprom
Description   :Function to write strings to eeprom
Return Data   :Writes given data into eeprom
****************************/
void Write_To_Eeprom(char add,String data)
{                                                          //check if data is null
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)                                                    //Writes given info per bit of storage
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   
                                                           //Fixes changes done in eeprom
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
      Write_To_Eeprom(DEV_ID_LOCATION,InData);                                        //declare response to handle from fn
      return 1;
    }
    else
    {
      EEPROM.write(DEV_ID_LOCATION,'\0');                                        //make function and add parameters--call once only
      return -1;                                                    // check eeprom write error return
    }
  }
  
  else if(InIdentifier=="FirstClientId")                                  //Storing First client id in eeprom
  {
    if(len<=40)
    {
      Write_To_Eeprom(FIRST_CLIENT_ID_LOCATION,InData);                                            //another fn that checks length and to eeprom...remove if..elseif.... reduce code size
      return 1;
    }
    else
    {
      EEPROM.write(FIRST_CLIENT_ID_LOCATION,'\0'); 
      return -1;
    }   
  }
  
  else if(InIdentifier=="SecondClientId")                                 //Storing Second client id in eeprom
  {
    if(len<=40)
    {
      Write_To_Eeprom(SECOND_CLIENT_ID_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(SECOND_CLIENT_ID_LOCATION,'\0'); 
      return -1;
    }
  }
  
  else if(InIdentifier=="ip_address")                                      //Storing Ip address in eeprom
  {
    if(len<=20)
    {
      Write_To_Eeprom(IP_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(IP_LOCATION,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="wifi_ssid")                                       //Storing wifi ssid in eeprom
  {
    if(len<=20)
    {
      Write_To_Eeprom(SSID_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(SSID_LOCATION,'\0'); 
      return -1;
    }
  }

  else if(InIdentifier=="wifi_password")                                   //Storing wifi password in eeprom
  {
    if(len<=20)
    {
      Write_To_Eeprom(PASSWORD_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(PASSWORD_LOCATION,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="timer_data")                                      //Storing timer data in eeprom
  {
    if(len<=8)
    {
      Write_To_Eeprom(TIMER_DATA_LOCATION,InData);
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
      Write_To_Eeprom(REPEAT_LOCATION,InData);
      return 1;
    }
    else
    {
      EEPROM.write(REPEAT_LOCATION,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="lower_threshold")                                 //Storing repeat data in eeprom
  {
    if(len<=1)
    {
      Write_To_Eeprom(LOWER_THRESHOLD,InData);
      return 1;
    }
    else
    {
      EEPROM.write(LOWER_THRESHOLD,'\0'); 
      return -1;
    }
  }
 
  else if(InIdentifier=="upper_threshold")                                 //Storing Actuator status in eeprom
  {
    if(len<=1)
    {
      Write_To_Eeprom(UPPER_THRESHOLD,InData);
      return 1;
    }
    else
    {
      EEPROM.write(UPPER_THRESHOLD,'\0'); 
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
  else if(OutIdentifier=="FirstClientId")                                 //Retrieving frist client id from eeprom
  {
    OutData=ReadFromEeprom(FIRST_CLIENT_ID_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="SecondClientId")                                //Retrieving second client id from eeprom
  {
    OutData=ReadFromEeprom(SECOND_CLIENT_ID_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="ip_address")                                     //Retrieving ip address from eeprom
  {
    OutData=ReadFromEeprom(IP_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="wifi_ssid")                                      //Retrieving wifi ssid from eeprom
  {
    OutData=ReadFromEeprom(SSID_LOCATION);
    return OutData;
  }
  else if(OutIdentifier=="wifi_password")                                  //Retrieving wifi password from eeprom
  {
    OutData=ReadFromEeprom(PASSWORD_LOCATION);
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
  else if(OutIdentifier=="lower_threshold")                                //Retrieving temporary repeat data from eeprom
  {
    OutData=ReadFromEeprom(LOWER_THRESHOLD);
    return OutData;
  }
  else if(OutIdentifier=="upper_threshold")                                //Retrieving actuator status from eeprom
  {
    OutData=ReadFromEeprom(UPPER_THRESHOLD);
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
  Write_To_Eeprom(172,"1");                                                 //Resets repeat info in eeprom
  return 1;
}
/****************************
Function Name :ResetRepeatData
Description   :Resets repeat data in eeprom
Return Data   :Returns 1 on success
****************************/
int clearEeprom(char add)
{
  int i;
  char data[100];
  int eeprom_length=0;
  unsigned char eeprom_address;
  eeprom_address=EEPROM.read(add);
  while(eeprom_address!= '\0' && eeprom_length<500)   
  {    
    eeprom_address=EEPROM.read(add+eeprom_length);
    EEPROM.write(add+eeprom_length,'\0');
    eeprom_length++;
  }
 
}

