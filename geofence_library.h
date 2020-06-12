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
  #ifndef GEOFENCE_LIBRARY_H_
  #define GEOFENCE_LIBRARY_H_
/*************C LIBRARIES********/
  #include <String.h>
  #include "Arduino.h"
/******declarations************/
  extern int  itration1,itration2,boundry;
  extern float dist;
  extern  int thresholdDistance;  
/*********************function declarartions****************************/
  extern int check_position(float thresholdDistance);
  extern float HaverSine(float lat1, float lon1, float lat2, float lon2);
 
#endif
