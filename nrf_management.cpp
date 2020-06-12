  
/**************************************************************************************
 * 
 * Name          : nrf_management.cpp
 * 
 * Version       : 1.0
 * 
 * Author        : Raji
 * 
 * Description   : This library is to initalize nrf module in arduino nano          
 *  
 *  WiFi Input        : {"ssid":"yyyyy","password":"xxxx"}              
 * 
 * Pin Discription: (nrf->arduino nano) 
 *                           Rx->Tx  
 *                           Tx->Rx
 *                           Vcc->Vcc
 *                           Gnd->Gnd.          
 **************************************************************************************/
  #include "nrf_management.h"
  #include "Arduino.h"
  #include<EEPROM.h>
  #include <String.h>
/***** INCLUDES LIBRARIES*******/
#include <Wire.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
/***********creating object******************/
RF24 radio(7, 8);
/********************************#define************************************/
  #define DELAY_10 10
  #define DELAY_100 100
  /***********declarations******************/
    byte sensor1Actuator,sensor2Actuator,sensor3Actuator;
    const uint64_t rAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };
    char* rAddresss[] = {"0x7878787878LL", "0xB3B4B5B6F1LL", "0xB3B4B5B6CDLL", "0xB3B4B5B6A3LL", "0xB3B4B5B60FLL", "0xB3B4B5B605LL" };
    char node_address[6][5];
    char incomingData11[35];
    char inBuiltSolenoid[10]="0x686868LL";
    char nodeMsg[35];
    byte pipeNum;
    char gatewayAddress[10]; //"OTSLITEGID001";
    String output_string="";
    String NUM;
    String copy_output_string="";
/***************************nrf_initialization******************
     FUNCTION:    nrf_initialization
     DISCRIPTION:  the nrf is inialized here which is normally 
                  done in void setup.this ia also used to set
                  the rf level 
     PARAMETERS:  int
     RETURNS:     returns which node has been 
**********************************************************8*/
int nrf_initialization(void){
    radio.begin();
    radio.setPALevel(RF24_PA_MIN);
    radio.setChannel(108); 
   
     Serial.print("init done");
  radio.openReadingPipe(0,rAddress[0]);
  radio.openReadingPipe(1,rAddress[1]);
  radio.openReadingPipe(2,rAddress[2]);
  radio.openReadingPipe(3,rAddress[3]);
  radio.openReadingPipe(4,rAddress[4]);
  radio.openReadingPipe(5,rAddress[5]);
  
  radio.startListening();
  
    return 1;
}
/***************************************************
      FUNCTION:    get_string_from_nrf
     DISCRIPTION:  this has a while loop andwhich keeps 
                   on checking for the input string from 
                   any nrf nodes
     PARAMETERS:  int
     RETURNS:     returns which node has been  
 ***************************************************/
char get_string_from_nrf(void){
  pipeNum = 0;
    while(radio.available(&pipeNum)){ //Check if received data
          Serial.print("KKKKKKKKKKK:");
          Serial.println(pipeNum);
          radio.read( &incomingData11, sizeof(incomingData11) ); //read one byte of data and store it in gotByte variable
          Serial.println(incomingData11);
          output_string="";
         output_string=incomingData11;
           Serial.println(output_string);
            send_nrf_input();
          return incomingData11;
    }
}
/***********************set_threshold**********************
      FUNCTION:    set_threshold
     DISCRIPTION:  get the threshold  information from gateway and sends
                   it to all the other nrf nodes from the gateway nrf
     PARAMETERS:  int
     RETURNS:     returns which node has been 
 ********************************************************/
int set_threshold(int lower_limit,int upper_limit){
                                 char node_data[35];
                                 // missing convert int to char
                                 char up_limit[3];
                                 char low_limit[3];
                                 
                                  strcpy(node_data,"{2000:");
                                  strcat(node_data,gatewayAddress);
                                  strcat(node_data,",");
                                   strcat(node_data,up_limit);
                                  strcat(node_data,":");
                                  strcat(node_data,low_limit);
                                  strcat(node_data,",}");
                                sendToNode(2,node_data);
                                delay(1000);
                         sendToNode(3,node_data);
                         delay(1000);
                          sendToNode(4,node_data);
}
/******************************************delete_node******************************
     FUNCTION:    delete_node
     DISCRIPTION:  get the delete information from gateway and delete's that
                   particular node from eeprom and changes the node back t0
                   5th node
     PARAMETERS:  int
     RETURNS:     returns which node has been deleted
**********************************************************************************/
int delete_node(int node_to_be_deleted){
              strcpy(nodeMsg,"{800:");
                 strcat(nodeMsg,gatewayAddress);
                strcat(nodeMsg,",");
              sendToNode(node_to_be_deleted,nodeMsg);
                   
                   return 1;
}
/**********************************mapping_and_unmapping***************************************8
     FUNCTION:    mapping_and_unmapping
     DISCRIPTION:  get the mapping and un-mapping  information from gateway and delete's that
                   particular pair  from eeprom and
     PARAMETERS:  int
     RETURNS:     returns its mapped succesfully or not
*****************/
int mapping_and_unmapping(int map_or_unmap,int solenoid_node,int sensor_node){ 
  /**********************************
                  if(map_or_unmap==1) // mapping
                  {
                   /***********************for inbuilt solenoid*****************************
                         if(solenoid_node==inBuiltSolenoid)
                            {
                             if(sensor_node==node_address[2])
                                {
                                  EEPROM.write(300,2);
                                   
                                   sensor1Actuator=2;
                                  } else if(sensor_node==node_address[3])
                                          {
                                            EEPROM.write(301,2);
                                              
                                              sensor2Actuator=2;
                                             } else if(sensor_node==node_address[4])
                                                    {
                                                      EEPROM.write(302,2);
                                                       
                                                        sensor3Actuator=2;
                             }
                           }
                /***********************for 0th solenoid*****************************
                            else if(solenoid_node==node_address[0])
                                   {
                                    if(sensor_node==node_address[2])
                                      {
                                        EEPROM.write(300,0);
                                         
                                        sensor1Actuator=0;
                                       } else if(sensor_node==node_address[3])
                                             {
                                                EEPROM.write(301,0);
                                                 
                                                sensor2Actuator=0;
                                               } else if(sensor_node==node_address[4])
                                                       {
                                                           EEPROM.write(302,0);
                                                             
                                                             sensor3Actuator=0;
                                                       }
                   
                                           }
                   /***********************for 1th solenoid***************************** 
                                            else if(solenoid_node ==node_address[1])
                                                 {
                                                    if(sensor_node==node_address[2])
                                                         {
                                                           EEPROM.write(300,1);
                                                              
                                                             sensor1Actuator=1;
                                                          } else if(sensor_node==node_address[3])
                                                                 {                 
                                                                     EEPROM.write(301,1);
                                                                      
                                                                     sensor2Actuator=1;
                                                                  } else if(sensor_node==node_address[4])
                                                                        {
                                                                              EEPROM.write(302,1);
                                                                               
                                                                              sensor3Actuator=1;
                                                                         }
                                                                         
                                                      }
                                                      }
                  /***********************unmapping*************************
                  } else if(map_or_unmap==0)
                          {
                            if(sensor_node==node_address[2])
                              {
                                  EEPROM.write(300,104);
                                   
                                  sensor1Actuator=104;
                               } else if(sensor_node==node_address[3])
                                      {
                                          EEPROM.write(301,104);
                                           
                                          sensor2Actuator=104;
                                       } else if(sensor_node==node_address[4])
                                             {
                                                 EEPROM.write(302,104);
                                                  
                                                 sensor3Actuator=104;
                                              }
                 
                     }*/
                     return 1;
                  }
                  
/********************************sendToNode**************************                  
 *  FUNCTION:      sendToNode
    DISCRIPTION:  get the char array of data and to which node to send
                   so this function send to that partucular node
    PARAMETERS:   bool
    RETURNS:     returns the success status of sending info to that particular
                  that is mentioned                
 *                   
 ******************************************************************************/
 bool sendToNode(byte xMitter, char transmitData[35]) {
    bool worked;                                   //variable to track if write was successful
    radio.stopListening();                         //Stop listening, start receiving data.
    radio.openWritingPipe(rAddress[xMitter]);     //Open writing pipe to the nRF24 that guessed the right number
    char charBuf[35];
    strcpy(charBuf,transmitData);
    if(!radio.write(&charBuf, sizeof(charBuf)))  worked = false; //write the correct number to the nRF24 module, and check that it was received
    else worked = true;                         //it was received
    radio.startListening();                     //Switch back to a receiver
    return worked;                              //return whether write was successful
}
/**************************** add_node******************************                  
    FUNCTION:       add_node
    DISCRIPTION:  get the char array of data and to which node to send
                   so this function send to that partucular node
    PARAMETERS:   int
    RETURNS:     returns the success status of sending info to that particular
                  that is mentioned                          
 *                   
 ***********************************************************/
int add_sensor_node( int unassigned_node){
  /******************add moisture sensor**************************/ 
  int i;         
          if(pipeNum==5)
          {
         
                
               char charBuf[3];
               NUM=String(unassigned_node);
               NUM.toCharArray(charBuf, 3);
               strcpy(nodeMsg,"{800:");
                 strcat(nodeMsg,gatewayAddress);
                strcat(nodeMsg,",");
                strcat(nodeMsg,charBuf);
                strcat(nodeMsg,"}");
               sendToNode(5,nodeMsg);
              
            }
           return unassigned_node;
}
/*****************************
******************************/
int add_solenoid_node(int unassigned_node){
  int i; 
       Serial.println("nodeMsg");
       Serial.println(pipeNum);
      if(pipeNum==5)
      {

               char charBuf[3];
               NUM=String(unassigned_node);
                NUM.toCharArray(charBuf, 3);
                radio.stopListening();
                 strcpy(nodeMsg,"{800:");
                 strcat(nodeMsg,gatewayAddress);
                strcat(nodeMsg,",");
                strcat(nodeMsg,charBuf);
                strcat(nodeMsg,":0}");
                delay(10000);
                
               sendToNode(5,nodeMsg);
            }
          
      
     
     return unassigned_node;              
}
/**************************************************************************** 
     FUNCTION:    add_node
    DISCRIPTION:  get the char array of data and to which node to send
                   so this function send to that partucular node
    PARAMETERS:   int
    RETURNS:     returns the success status of sending info to that particular
                  that is mentioned   
***************************************************************************/
int  get_solenoid_status(String node_address,String get_solenoid){
 
       // if(node_address==node_address[0])
        //{
         // Serial.println("inside if");
          // sensor1Status=get_solenoid.toInt();
        // } else if(node_address==node_address[1])
         //{
          // sendToActuatorNode(sensor2Actuator,NrfAction2);
           //sensor2Status=get_solenoid.toInt();
        // }
      
      return 1;
}
/********************************control_solenoid********************************* 
      FUNCTION:   control_solenoid
    DISCRIPTION:  get the char array of data and to which node to send
                   so this function send to that partucular node
    PARAMETERS:   int
    RETURNS:     returns the success status of sending info to that particular
                  that is mentioned   
***************************************************************/
int control_solenoid(String node_address,String solenoid_control){    
//if(node_address==node_address[1]){
       //    sendToActuatorNode(sensor1Actuator,solenoid_control);
        // }       

//if(node_address==node_address[2]){
          // sendToActuatorNode(sensor1Actuator,solenoid_control);
         //} 
//if(node_address==node_address[3])
  //       {
    //       sendToActuatorNode(sensor2Actuator,solenoid_control);
      //   }
//else if(node_address==node_address[4])
  //       {
    //       sendToActuatorNode(sensor3Actuator,solenoid_control);
      //   }
///else if(node_address==node_address[5])
   //      {
     //      sendToActuatorNode(sensor3Actuator,solenoid_control);
       //  }
       return 1;
      }
/*******************************************send_wss_input******************************
     FUNCTION:    send_bluetooth_input

     DISCRIPTION: Takes the string from wss and gives it back to the main code when this 
                  function is called
     PARAMETERS:  int

     RETURNS:     returns the string from wss
**********************************************************************************/
String send_nrf_input(){
  String null_string="null";
  int counter=0;
 if (output_string != "") {
   Serial.println("send_nrf_input was");
  Serial.println(output_string);
   copy_output_string="";
   copy_output_string=copy_output_string+output_string;
    counter++;
    if(counter==2){
     output_string="";
     counter=0;
    }
  return copy_output_string;
 }
 if (output_string == "") {
  return  null_string;
 }
}
bool sendToActuatorNode(byte xMitter, String state) {
  if(xMitter!=104)
  {
    bool worked; //variable to track if write was successful
    radio.stopListening(); //Stop listening, start receiving data.
    radio.openWritingPipe(rAddress[xMitter]); //Open writing pipe to the nRF24 that guessed the right number
      
char charBuf[3];
state.toCharArray(charBuf, 3);
Serial.println(charBuf);
strcpy(nodeMsg,"{500:");
strcat(nodeMsg,gatewayAddress);
strcat(nodeMsg,",");
strcat(nodeMsg,charBuf);
strcat(nodeMsg,":1}");
      if(!radio.write(&nodeMsg, sizeof(nodeMsg)))  worked = false; //write the correct number to the nRF24 module, and check that it was received
    else worked = true; //it was received
    radio.startListening(); //Switch back to a receiver
    return worked;  //return whether write was successful
  }
}
