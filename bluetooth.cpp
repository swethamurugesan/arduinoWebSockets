/**************************************************************************************
 * 
 * Name          : bluetooth.cpp
 * 
 * Version       : 1.0
 * 
 * Author        : Raji
 * 
 * Description   : This library is to connect bluetooth module  with nodemcu12e
 *                  in order to connect to wifi and ip address by sending wifi 
 *                  credentials via bluetooth of SWMS          
 *  
 *  WiFi Input        : {"ssid":"yyyyy","password":"xxxx"}              
 * 
 * Pin Discription: (bluetooth->nodemcu12E) 
 *                           Rx->Tx  
 *                           Tx->Rx
 *                           Vcc->Vcc
 *                           Gnd->Gnd.          
 **************************************************************************************/
  #include "bluetooth.h"
  #include "Arduino.h"
  #include "BluetoothSerial.h"
  #include<EEPROM.h>
  #include <String.h>
  /********************************#define************************************/
  #define DELAY_10 10
  BluetoothSerial ESP_BT; 
  #define DELAY_100 100
/******string declarations************/
  String  input_bluetooth_string;
  String  output_bluetooth_string;

/*******************************************bluetooth***********************************************
   * FUNCTION   :  bluetooth
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
String bluetooth_connect(){

/***********************************************inialize the bluetooth variables*****************************************************/
  input_bluetooth_string = "";
  output_bluetooth_string="";


/*****************************************************Read the string from the bluetooth*****************************************/   
  while (ESP_BT.available()) {
       {
       delay(DELAY_10);                                                    //small delay to allow input buffer to fill
       char get_string = ESP_BT.read();                           //gets one byte from serial buffer
      /***breaks out of capture loop to print readstring***/
      input_bluetooth_string += get_string;
      }                                                             //makes the string readString 

  if  (input_bluetooth_string.length() > 0  ) 
      {
         Serial.println(input_bluetooth_string);                     //prints string to serial port out
         Serial.println( "the input string from bluetooth is" );
/*****************************************************JSON pharsing*****************************************/
        delay(DELAY_100);
        output_bluetooth_string="";
        output_bluetooth_string=output_bluetooth_string+input_bluetooth_string;
/*****************************CLEAR THE INPUT TEXT OF BLUETOOTH***********************/
        input_bluetooth_string="";                                                 //clears variable for new input
      }

  return output_bluetooth_string; 
}
}
/*******************************************bluetooth_initialization***********************************************
   * FUNCTION   :  bluetooth_initialization
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
int bluetooth_initialization(){
/***********************************initialize bluetooth*************************************************/
 Serial.println("hii im ble");
    ESP_BT.begin("Vehicle_tracking"); //Name of your Bluetooth Signal
    return 1;
}

