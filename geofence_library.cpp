/**************************************************************************************
 * 
 * Name          : geofence_libray.cpp
 * 
 * Version       : 1.0
 * 
 * Author        : Raji
 * 
 * Description   : This library is to calculate the device is inside or outside of boundry         
 *        
 **************************************************************************************/
  #include "geofence_library.h"
  #include "Arduino.h"
  #include<EEPROM.h>
  #include <String.h>
  /********************************#define************************************/
  #define DELAY_10 10
  #define DELAY_100 100
  /******declarations************/
   int  itration1,itration2,boundry;
   float dist;
   int thresholdDistance; 
  /*******************************************action_check***********************************************
   * FUNCTION   :  check_position
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
int check_position(float thresholdDistance){
  int return_checkposition=-1;
  if((dist>thresholdDistance)&&(itration1==0))
 { 
  itration1++;
  itration2=0;
  digitalWrite(8,HIGH); 
  delay(2000);

  Serial.println(thresholdDistance);
  Serial.println(dist);
  Serial.println("******************************************************outide boundary");
  return_checkposition=0;
 }
  if((dist<thresholdDistance)&&(itration2==0))
 {
   itration1=0;
   itration2++;
  boundry=1;
  Serial.println("******************************************************inside boundary");
  digitalWrite(8,LOW); 
  return_checkposition=1;
 }
 delay(100);
 Serial.println(return_checkposition);
 return return_checkposition;
 }
 /*******************************************action_check***********************************************
   * FUNCTION   : HaverSine 
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
 float HaverSine(float lat1, float lon1, float lat2, float lon2)
{
    Serial.println("lat1,lon1 of boundry check is");
  Serial.println(lat1);
  Serial.println(lon1);
  Serial.println(lat2);
  Serial.println(lon2);
 float ToRad = PI / 180.0;
 float R = 6371000;   // radius earth in Km
 
 float dLat = (lat2-lat1) * ToRad;
 float dLon = (lon2-lon1) * ToRad; 
 
 float a = sin(dLat/2) * sin(dLat/2) +
       cos(lat1 * ToRad) * cos(lat2 * ToRad) * 
       sin(dLon/2) * sin(dLon/2); 
       
 float c = 2 * atan2(sqrt(a), sqrt(1-a)); 
 
 float d = R * c;
 //Serial.println(d, 6);
 return d;
}
