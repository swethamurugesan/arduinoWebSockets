/***** INCLUDES LIBRARIES*******/
#include <SPI.h>                                     //Call SPI library so you can communicate with the nRF24L01+
#include <nRF24L01.h>                               //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <EEPROM.h>
#include <RF24.h>                                  //nRF2401 libarary found at https://github.com/tmrh20/RF24/
/*******************************/
#include "jsonLibrary.h"
#include "eeprom_library.h"
/************************************/
#define WHICH_NODE 6                         // must be a number from 1 - 6 identifying the PTX node
/*******************************************/
RF24 radio(7, 8);                 // Create your nRF24 object or wireless SPI connection
StaticJsonDocument<400> json_message;     //StaticJsonDocument<N> allocates memory on the stack
/***************************************/
 int itration2; 
 int x111=0;
 int p111=0;
 int this_node=5;
 int moisture_value;
 int state=0;
 int interupt_state=0;
 int upper_limit=90;
 int lower_limit=50;
 char node_message[100];
 char node_data[35];
 char nodeAddress[]="ONS001";
 char incoming_data[100];
 char dest[100];
 
 const uint64_t wAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
 char* rAddresss[] = {"0x7878787878LL", "0xB3B4B5B6F1LL", "0xB3B4B5B6CDLL", "0xB3B4B5B6A3LL", "0xB3B4B5B60FLL", "0xB3B4B5B605LL" };
 uint64_t PTXpipe = wAddress[ WHICH_NODE - 1 ];    // Pupper_limitls the address from the above array for this node's pipe
 byte counter = 1;                                 //used to count the packets sent
 bool done = false;                                //used to know when to stop sending -packets
 String NrfAction1,NrfAction2,NrfAction3,Node,Nrftype,Type;
 String gateway_address="123";
//upper_limit=EEPROM.read(100);
//LL=EEPROM.read(101);
void setup()  

{
  PTXpipe=wAddress[this_node];
  Serial.begin(115200);                         //start serial to communicate process
  radio.begin();                                //Start the nRF24 module
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(108);                             // the higher channels tend to be more "open"
/******************************read the node number from eeprom*********************************************/
  if(EEPROM.read(5))
  {
   this_node=EEPROM.read(0);
   PTXpipe = wAddress[this_node]; 
//   gateway_address=ReadFromEeprom(30);
  }
  radio.openReadingPipe(0,PTXpipe);        //open reading or receive pipe
  radio.startListening();                  //go into transmit mode
  delay(1000);
}

void loop()  

{

//===================interrupt 2 detectecd which is disconnect===============//
delay(1100);
 if((state==1)&&(interupt_state==0))
   {
    Serial.println("Interrupt2 Detected"); 
//==================== disconecct ================//
 
strcpy(node_data,"{700:");
strcat(node_data,nodeAddress);
strcat(node_data,",}"); 
  if(sendToNode(this_node,node_data)) {
    interupt_state=1;
  }
}
  
  
 else{
  while(1)
  {      delay(900);

   //moisture_value= analogRead(A0);
   //moisture_value= moisture_value/10;
   //Serial.println(moisture_value);
   /// delete only for testing******************/
   moisture_value=99;
        radio.stopListening();
    strcpy(node_message,"");
    strcpy(node_message,"{\"NA\":\"700\",\"ID\":\"453\"}");
      radio.openWritingPipe(PTXpipe);        //open writing or transmit pipe
     if (!radio.write(&node_message, sizeof(node_message))){  //if the write fails let the user know over serial monitor

         Serial.println(" failed");      

     }

     else { //if the write was successful 

          Serial.print("Success :");

          Serial.println(node_message);
     }

     radio.startListening();   
   //=======================upper limit================================================//
   if((moisture_value>upper_limit)&&(p111==0))
   {
     p111++;
     x111=0;
     radio.stopListening();
    strcpy(node_message,"");
    strcpy(node_message,"{400:0xB3B4B5B6CDLL,1:0}");
      radio.openWritingPipe(PTXpipe);        //open writing or transmit pipe
     if (!radio.write(&node_message, sizeof(node_message))){  //if the write fails let the user know over serial monitor

         Serial.println(" failed");      

     }

     else { //if the write was successful 

          Serial.print("Success :");

          Serial.println(node_message);
     }

     radio.startListening();   

delay(1000);
   }
   //=======================lower limit================================================//
  if((moisture_value<lower_limit)&&( x111==0))
   {
       x111++;
       p111=0;
      radio.stopListening();
      strcpy(node_message,"");
      strcpy(node_message,"{400:0xB3B4B5B6CDLL,0:0}");
      radio.openWritingPipe(PTXpipe);        //open writing or transmit pipe
     if (!radio.write(&node_message, sizeof(node_message))){  //if the write fails let the user know over serial monitor

         Serial.println(" failed");      

     }

     else {                                    //if the write was successful 

          Serial.print("Success :");

          Serial.println(node_message);
}
delay(1000);
   }

/*******************************************NRF RECIVIVER***********************************************/                                             
while ( radio.available() ) { //run while no receive data and not timed out
      radio.read( &incoming_data, sizeof(incoming_data) );
      strncpy(dest,incoming_data, sizeof(incoming_data));
      Serial.println(incoming_data);
      delay(200);  
      NRF_check(dest);
        }
          delay(1000);        
/*******************************************NRF RECIVIVER***********************************************/   
         if(state==1)
          {
            break;
          }
  
 
    delay(100);
  }
 } 
   }



void NRF_check(char *commands){



   Serial.println("commands");
   Serial.println(commands);
  /******************************parse json action********************************************/
  json_message = json_parser(commands);
  String NrfAction1= json_message["action"];
  String Node = json_message["descp"];
   
       Serial.println("NrfAction1");
    Serial.println(NrfAction1);
    Serial.println("Node");
     Serial.println(Node);
     Serial.println("NrfAction2");
     Serial.println(NrfAction2);
          Serial.println("type");
     Serial.println(Type);
        Serial.println("NrfAction3");
         Serial.println(NrfAction3);
          Serial.println("id");
     Serial.println(Nrftype);
   delay(99);
              
    if(NrfAction1=="800")
      {
        gateway_address=Node;
        eeprom_write(30,gateway_address);
        this_node=NrfAction2.toInt();
        EEPROM.write(0,this_node);
        EEPROM.write(5,1);
        radio.stopListening();
      PTXpipe = wAddress[this_node];
        radio.openReadingPipe(0,PTXpipe);
       radio.startListening();
      }
      else if(NrfAction1=="1000")
      {
       gateway_address=""; 
        radio.stopListening();
        clearEeprom(30);
          EEPROM.write(5,0);
            EEPROM.write(0,5);
      PTXpipe = wAddress[5];
        radio.openReadingPipe(0,PTXpipe);
       radio.startListening();
      }
      else if(NrfAction1=="2000")
      {
        upper_limit=NrfAction2.toInt();
        EEPROM.write(100,upper_limit);
        lower_limit=Type.toInt();
        EEPROM.write(101,lower_limit);
      }
        
      }


 bool sendToNode(byte xMitter, char* transmitData) {
    bool worked; //variable to track if write was successful
    radio.stopListening(); //Stop listening, start receiving data.
    radio.openWritingPipe(wAddress[xMitter]); //Open writing pipe to the nRF24 that guessed the right number
    if(!radio.write(&transmitData, sizeof(transmitData)))  worked = false; //write the correct number to the nRF24 module, and check that it was received
    else worked = true; //it was received
    radio.startListening(); //Switch back to a receiver
    return worked;  //return whether write was successful
}




/***********************************************************************
Function name : WriteToEeprom
Description   : To write given data into controller memory.
Parameters    : Address fro storage and data to be stored.
***********************************************************************/
void eeprom_write(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   
//  EEPROM.commit();
}


 void handleInterrupt() { 
   
   state=!state;

}
