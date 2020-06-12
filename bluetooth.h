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
 **************************************************************************************/
  #ifndef BLUETOOTH_H_
  #define BLUETOOTH_H_
/*************C LIBRARIES********/
  #include <String.h>
  #include<EEPROM.h>
  #include "Arduino.h"
  #include "BluetoothSerial.h"
/******string declarations************/
  extern String  input_bluetooth_string;
  extern String  output_bluetooth_string;  
/*********************function declarartions****************************/
  extern String bluetooth_connect(void);
  extern int bluetooth_initialization(void);
#endif
