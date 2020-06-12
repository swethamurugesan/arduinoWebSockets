/**************************************************************************************
 * 
 * Name        : swms regular
 * 
 * Version     : 1.0
 * 
 * Author      : Raji
 * 
 * controller  : arduino nano 
 * 
 * Description : This is the Smart Water Management System and is adapted to work with
 *               websocket server.this system has two sensor(dht11) 
 *               and one actuators.this can be used for potted plants and gardening since 
 *               it is wired system.
 *                             
 **************************************************************************************/
 /***** INCLUDES LIBRARIES*******/
 #include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
/*******************************/
#include "nrf_management.h"
#include "uart_library.h"
#include "jsonLibrary.h"
#include "eeprom_library.h"
/*******************************************/
StaticJsonDocument<100> json_message;//StaticJsonDocument<N> allocates memory on the stack
/****************declarations*****************/
   int itration1,itration2,itration3;
   int unassigned_node;
  char commands[10],char_clear[10];
  char nrf_commands[30];
  char nrf_id_array[6][10];
  String string_clear_eeprom="";
  String nrf_input;
  String solenoid_control,get_solenoid;
  String string_node_id="";
  String uart_input;
  String clear_eeprom="";
  String upper_threshold="50";
  String lower_threshold="90";

void setup() {
   /*********************************baudrate of serial communication*******************************************/
    Serial.begin(115200);
  /**********************************NRF  INITALIZATION*************************************************/
    SPI.begin();
    nrf_initialization();
       /*****************mode is automatric or manual***********/
 Serial.println(" JsonData2sz");
// Serial.println(EEPROM.read(0));
// sensor1Actuator=EEPROM.read(300);
 //sensor2Actuator=EEPROM.read(301);
 //sensor3Actuator=EEPROM.read(302);
 /****************************clear the eeprom*************************/
  for(itration1=0;itration1<5;itration1++)
 {
  Write_To_Eeprom(eepromLocation[itration1],string_clear_eeprom);
  string_clear_eeprom.toCharArray(char_clear, 10);
   strcpy(nrf_id_array[itration1],char_clear);
    Serial.println(nrf_id_array[itration2]);
 }
/****************************read eeprom and write************************/
 for(itration2=0;itration2<5;itration2++)
 {
  char nrf_id[10];
  ReadFromEeprom(eepromLocation[itration2]).toCharArray(nrf_id, 10);
 strcpy(nrf_id_array[itration2],nrf_id);
 Serial.println(nrf_id_array[itration2]);
 }

}

void loop() {
/*******************************************NRF RECIVIVER***********************************************/                                             
          get_string_from_nrf(); 
          nrf_input = send_nrf_input();   
          if(nrf_input  != "null"){
            strcpy(nrf_commands, nrf_input.c_str());
           NRF_check(nrf_commands);
           nrf_commands[30]="";
           nrf_input ="null";
           }
        
/*************************************************************************************************/
           uart_input=uartString();
if((uart_input)!="")
{
  Serial.println(uart_input);
  strcpy(commands, uart_input.c_str());
     action_check(commands);
}  
}
/*******************************************action_check******************************
     FUNCTION:  Action_check
     DISCRIPTION:This void function does json pharsing and switch statement for the get text from web socket server and gives
     acknowledgement back to the web socket server and does the work accordingly to each command
     1.Manual /Timer
     2.Motor on/off
     3.device details
     4.Getting time for RTC
     5.Getting client ID
     PARAMETERS: int
     RETURNS:action number
   **********************************************************************************/
int action_check(char *commands) {

  Serial.println("************action from the gateway***********");
  Serial.println(commands);//CHECK FOR NULL
/******************************parse json action********************************************
  json_message = json_parser(commands);
  int action = json_message["action"];
  String change_time = json_message["descp"];
  String json_upper = json_message["LL"];
    String json_lower = json_message["UL"];
/******************************parse json timer********************************************
  String timer = json_message["UL"];

/******************************parse json rtc update********************************************/
int action =90;
  switch (action) {
    /**************************************************************/
    case 107: {
      //mapping_and_unmapping(map_or_unmap,solenoid_node,sensor_node)
      mapping_and_unmapping(1,1,2);
     
    }
      break;
   /****************************delete node*********************************/
     case 108: {
      delete_node(2);
      clearEeprom(eepromLocation[2]);
    
    }
     break;
   /*************************************************************/
    case 109: {
      //set_threshold(int lower_limit,int upper_limit)
      set_threshold(50,90);
      /***********************store in eeprom********************************/
      StoreInfo("lower_threshold",lower_threshold);
      StoreInfo("upper_threshold",lower_threshold);
     
    }
     break;
    /*************************************************************/
    default:{
        Serial.println("wrong");
            }
                   break;
  }
  }
    
/*********************************************************    
    FUNCTION:       NRF_check
    
    DISCRIPTION:  get the char array of data and to which node to send
                   so this function send to that partucular node
    PARAMETERS:   int
    RETURNS:     returns the success status of sending info to that particular
                  that is mentioned
********************************************************************/
void NRF_check(char *nrf_commands){
Serial.println("commands");
Serial.println("************action from the nodes***********");
  Serial.println(nrf_commands);//CHECK FOR NULL
/******************************parse json action********************************************/
  json_message = json_parser(nrf_commands);
  int nrf_action = json_message["NA"];
  Serial.println(nrf_action);
  String node_address_json = json_message["ID"];
  string_node_id="";
  string_node_id =string_node_id +node_address_json;
  Serial.println("string  node id");
  Serial.println(string_node_id);
/******************************parse json timer********************************************/
  String string_status = json_message["S"];
  solenoid_control=solenoid_control+string_status;
    String response_solenoid = json_message["SS"];
    get_solenoid="";
    get_solenoid=get_solenoid+response_solenoid;
/*************************************************/
Serial.println(nrf_action);
switch (nrf_action){
  case 600:
      { 
        get_solenoid_status(string_node_id,get_solenoid);  
      } 
       break;
/*************************************************/ 
    case 400:
      { 
        if(string_node_id==nrf_id_array[3]){
         control_solenoid(string_node_id,solenoid_control);
        }
      }
       break;
/******************add moisture sensor**************************/
    case 700:
      {
        Serial.println("adding node");
        delay(1000);
       for(itration3=0;itration3<5;itration3++)
          { Serial.println("node address");
            Serial.println(nrf_id_array[itration3]);
            if(nrf_id_array[itration3]==" ")
            {  Serial.println("came inside loop");
              delay(100);
               unassigned_node =itration3;
               Serial.println("unassigned  node");
               Serial.println(unassigned_node);
               Serial.println(string_node_id);
               char char_array[10];
               string_node_id.toCharArray(char_array, 10);
               strcpy(nrf_id_array[itration3],char_array);
               String json;
               String slash="\"";
                json="{\"resp\":\"106\" ,\"compId\""":"+slash+String(node_address[itration3])+slash+"}";
                             Serial.println(json);
                   
                
                  Write_To_Eeprom(eepromLocation[itration3],String(nrf_id_array[itration3]));
                  add_sensor_node(unassigned_node);
            }
          }
      }
    
     
      break;
/******************add solenoid**************************/
     case 900:
     {
           for(itration3=0;itration3<5;itration3++)
          {
            if(node_address[itration3]=="104")
            {
            
               unassigned_node=itration3;// it can be from 1-5
               char char_arrary[10];
               string_node_id.toCharArray(char_arrary, 10);//device id to array
               strcpy(nrf_id_array[itration3],char_arrary);  //copy that array to node_address
              Write_To_Eeprom(eepromLocation[itration3],String(nrf_id_array[itration3]));//write in 
     add_solenoid_node(unassigned_node);
     }
          }
     }
      break;
/******************default**************************/
     default: {

      }
      break;
}    
}
