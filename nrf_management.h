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
  #ifndef NRF_MANAGEMENT_H_
  #define NRF_MANAGEMENT_H_
/*************C LIBRARIES********/
  #include <String.h>
   /***** INCLUDES LIBRARIES*******/
#include <Wire.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
  /*****************************/
   extern const uint64_t rAddress[];
   extern char* rAddresss[];
   extern String output_string;
   extern String NUM;
   extern String copy_output_string;
   extern char node_address[6][5];
   extern char inBuiltSolenoid[10];
   extern char nodeMsg[35];
   extern byte pipeNum;
   extern char incomingData11[35];
   extern char gatewayAddress[10];
   extern  byte sensor1Actuator,sensor2Actuator,sensor3Actuator;
  /*****************************/
  extern int set_threshold(int lower_limit,int upper_limit);
  extern int nrf_initialization(void);
  extern char get_string_from_nrf(void);
  extern String send_nrf_input(void);
  extern int delete_node(int node_to_be_deleted);
  extern int add_sensor_node(int unassigned_node);
  extern int add_solenoid_node(int unassigned_node);
  extern int control_solenoid(String node_address ,String solenoid_control);
  extern int get_solenoid_status(String node_address,String get_solenoid);
  extern int mapping_and_unmapping(int map_or_unmap,int solenoid_node,int sensor_node);
  extern bool sendToNode(byte xMitter, char transmitData[35]);
   
#endif
