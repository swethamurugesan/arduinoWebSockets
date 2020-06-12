/******************************************************************************************************
 * Name:Vehicle Tracking
 
 * Version: 1.2

 * Author: Raji
 
 * Description: Main engine for VT (Vehicle Tracking), with one inbuit accelarometer and one GPS. 
 *              Can be used independently to track vehicles mostly used for farmming.
 *****************************************************************************************************/
/**********Libraries**********/
#include <TinyGPS++.h>
#include<EEPROM.h>
#include "String.h"
/*======================additional libraries========================*/
#include "geofence_library.h"
#include "sd_card_library.h"
#include "eeprom_library.h"
#include "timer.h"
#include "bluetooth.h"
#include "jsonLibrary.h"
/********************************objects*********************************/
/***********Instances************/
TinyGPSPlus gps;
StaticJsonDocument<400> json_message;
HardwareSerial GPSSerial(2);
HardwareSerial GSMSerial(1);
/*****************************************/
#define DRUM1 34
#define STATUS 33
#define DELAY_100 100
#define DELAY_1000 1000
#define DELAY_5000 5000
#define DELAY_200 200
#define DELAY_500 500
#define DELAY_2000 2000
#define THRESHOLD 120
#define ERROR_CASE -1
/*********Pin Assignment***********/
int sense_Pin=A0;
int vibr_pin=10;
int LED_Pin=13;
int  itration11,itration4,itration10=0;
byte vibration_value = 0;
/*******Global Variables********/
/*******int********/
int speed_counter=0;
int got_updated_time;
int transfer_complete=0;
int vibration_state=0;
int vehicle_status=0;
int iteration1=0;
int device_speed=0;
int clear_variable;
/****************float and long**********************/
float  homeLat;
float homeLon;
float current_lattitude;
float current_longitude;
unsigned long time_now=0;
/**************string and char***********************/
String string_lattitude,radius,string_longitude;
String bluetooth_string;
String timer_alarm;
String apn="airtelgprs.com";;
String inString = "";  
String slash="\"";
String url="";
String H,M,S;
String response_message="";
String Did="VTSGID001";
String communication_number="0000000000";
String Dsnum="6366616417";
String Rnum="8220480192";
char incoming_message[500];
/*************void functions***************/
void JsonParsing(void);
void isr1(void);
void serialE_event(void);
/**********************************************************************************************/
void setup()
{
/***************SERIAL BEGIN***********************/
    Serial.begin(9600);
/****************************GPS serial*********************************************/
    GPSSerial.begin(9600, SERIAL_8N1, 16, 17);
/********************************GSM serial*****************************************/
    GSMSerial.begin(9600, SERIAL_8N1,4, 2, false); //rx,tx
/***********************************eeprom initialization and read data*******************************/
    EEPROM.begin(512); 
/***********************************initialize bluetooth*************************************************/
    bluetooth_initialization(); //Name of your Bluetooth Signal
/*****************************SD CARD***********************************/
    int sd_value = sd_initialization();
/***************open the text file***********************/
if(sd_value !=  ERROR_CASE){
 
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt","\r\n");
  }

  else {
    Serial.println("File already exists");  
  }
  file.close();
}
/***************PINMODE DECLARATION***********************/
  pinMode(DRUM1,INPUT);  
  pinMode(STATUS,OUTPUT);

/**********************************RTC INITALIZATION*******************************************/
  int rtc_init = rtc_initialization();
  if (rtc_init != ERROR_CASE) {
    Serial.println("RTC initialization sucess");
    /**********************RTC Timer************/
    delay(1000);
    int get_time_return = get_time();
    /**********************retrive timer data from eeprom************/
    //timer_alarm = RetrieveInfo("timer_data");
    timer_alarm="11123311";
  /**********************pass to decode the timer************/
  Serial.println("given to decoder");
  DecodeTimers(timer_alarm);
  }
      delay(15000);
/***************EEPROM***********************/                                                       /*Read DeviceID from controller memory*/
  //communication_number=RetrieveInfo("communication_number"); 
  communication_number="6361215008";/*Read Communication number from controller memory*/
  delay(10);                                               /*Read Register number from controller memory*/   
  Serial.println(communication_number);
 /***********************retrive apn*******************************/ 
  //apn=RetrieveInfo("apn");      
  apn="airtelgprs.com";/*Read Communication number from controller memory*/
  delay(10);                                               /*Read Register number from controller memory*/   
 Serial.println(apn);
/***************************if no communication number*****************************/
  if(communication_number=="0000000000")                                                         /*Start-up sequence when comm number is not available(Request comm. center for comm number)*/
 {
   // get_time();
    
   
 }
                                                                           /*Start-up sequence when communication number is available*/
  else {

    /*********************CONNECTION HI MESSAGE*********************/
   response_message="${\"action\":\"REG_DEVICE\",\"deviceId\":\""+String(Did)+"}$";
   Serial.println(response_message);
   sms_gsm(response_message); 
   

  }
  /***************************************apn availability********************************************/
  /***************************no apn available*****************************/
  if(apn=="0000000000")                                                         /*Start-up sequence when comm number is not available(Request comm. center for comm number)*/
 {
   
    response_message="${\"action\":\"REG_DEVICE\",\"deviceId\":\""+String(Did)+"}$";
   sms_gsm(response_message); 
   
 }
  if(apn!="0000000000")                                                         /*Start-up sequence when comm number is not available(Request comm. center for comm number)*/
 {
                                                                          /*Start-up sequence when communication number is available*/

    /*********************CONNECTION HI MESSAGE*********************/
    response_message="${\"action\":\"REG_DEVICE\",\"deviceId\":\""+String(Did)+"}$";
    sms_gsm(response_message);

  }
 }
 void loop()
 {
/*******************************Gps******************************/ 
  while (GPSSerial.available() > 0) {  
              
/*********************start gps and get credentials****************/         
           char get_gps_credentials = GPSSerial.read();
            gps.encode(get_gps_credentials);
            if (gps.location.isUpdated()) {
/*******************************get lattitude******************************/
               current_lattitude=(gps.location.lat());
               Serial.print(current_lattitude,6);   
/*******************************get longitude******************************/ 
               current_longitude=(gps.location.lng());
               Serial.print(current_longitude,6);              
/*******************************get alttitude******************************/
               Serial.println("altitu");
               Serial.println(gps.altitude.meters());
/*******************************get speed******************************/
               Serial.println("speed");
               device_speed=(gps.speed.kmph());
              Serial.println(device_speed);
         }
      }
     
  
delay(DELAY_1000);

/***********************************************retrive all info****************************************************/
 //string_lattitude = RetrieveInfo("home_lattitude");
 //string_longitude = RetrieveInfo("home_longitude");
 //radius = RetrieveInfo("radius");
 string_lattitude ="12.932703018188";
 string_longitude = "77.586242675781";
 radius = "1000";
    
    homeLat=string_lattitude.toFloat();
    homeLon=string_longitude.toFloat();
    thresholdDistance=radius.toFloat();
    
    dist = HaverSine(homeLat,homeLon,current_lattitude,current_longitude);
    Serial.println("dist:");
    Serial.println(dist);
 
    Serial.println("the Radius iiiiiiiis inside threshold");
    Serial.println(thresholdDistance);
    boundry = check_position(thresholdDistance);
    delay(100);
/******************************************got sms from serial port*****************************************************/ 
    if(transfer_complete==1)                                                            /*Check if message has been received*/
  {
    transfer_complete=0;
    Serial.println("go to action");
    
    check_message(incoming_message);
    for(clear_variable=0;clear_variable<500;clear_variable++)                       /*Clear array for next message from comm. center*/
    {
      incoming_message[clear_variable]='\0';
    }
     check_message(incoming_message);
    
  }
                                                   /*Turn on comm. LED*/
      serial_event();
        
  /*****************************************************Read the string from the bluetooth**************************************  
               bluetooth_string = bluetooth_connect();   //call bluetooth function
  /*********************************bluetooth string is not a null string *******************************************
         if (bluetooth_string!=" ") {
 /**********************send bluetooth string for json parsing and stroring in eeprom********************************
               send_bluetooth_input(bluetooth_string);
         } 
 /****************************************got updated time***************************************************/   
 if (1 == got_updated_time) { 

    String string_repeat_mode = RetrieveInfo("RepeatData");
    repeat_mode = string_repeat_mode.toInt();
    Serial.println(repeat);
     
    int boundry_status = check_alarm_timer(STATUS, repeat_mode);
    if (1 == boundry_status){
        send_all_data_to_cloud();
    }
    if (ERROR_CASE != boundry_status) {
      Serial.println("no timer");

    }
  }
/******************************************************************************************************/
 }
/*********************************************************************
Function name : serial_event

Description   : To handle incoming uart communication form GSM module.

Parameters    : Returns Json string acquired form GSM text message.
*********************************************************************/

void serial_event()
{Serial.println("goteeeeeeeeee serial event");
  while(GSMSerial.available())
  {  GSMSerial.println("goteeeeeeeeee serial event");
     Serial.println("goteeeeeeeeee serial event");
    if(GSMSerial.find("#"))
    {GSMSerial.println("got serial event");
      delay(DELAY_1000);
      while (GSMSerial.available())
      {
        char inChar=GSMSerial.read();
        incoming_message[iteration1++]=inChar;
        if(inChar=='#')
        {
          iteration1=0;
          transfer_complete=1;
          Serial.println("serial event over");
          Serial.println(incoming_message);
          return;
        }
      }
    }
  }
  
}
/*******************************************action_check***********************************************
   * FUNCTION   :  send_all_data_to_cloud
   * 
   * DISCRIPTION: This function does main part of calculation like speed,vibration and boundry and send 
   *              to the cloud every 4 minute.
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : returns the vehicle status
****************************************************************************************************/
int send_all_data_to_cloud(){   
/*********check speed***********************/ 
 if(device_speed<5){
  device_speed=0;
  speed_counter++;
  Serial.println(speed_counter);
 }
 /*********speed increased***********************/
 if(device_speed>5)
 {
  speed_counter=0;
 }
 /*********no speed and check id IDLE or not***********************/
 if(speed_counter>30)
 {
   /*********vibration is there***********************/
 /**************KEEP VIBRATION CONSTANT **********************/ 
 delay(DELAY_2000);
 vibration_value = analogRead(DRUM1);
Serial.println("**************************************************valuee");
Serial.println(vibration_value);
  if(vibration_value > THRESHOLD) { Serial.println("**************************************************HARD");

   vibration_state=1; 
}
/*********vibration is NOT there***********************/
 if(vibration_value < THRESHOLD) { 
 Serial.println("**************************************************SOFT");
   vibration_state=0;  
   }
 /**************CHECK FOR angle **********************/
 
    if(millis() > time_now + 30000){
     time_now = millis();
      /**************running **********************/
     if((vibration_state==1)&&(device_speed>1)){
      vehicle_status=2;
     }
     /**************idle **********************/
     if((vibration_state==1)&&(device_speed<2)){
      vehicle_status=1;
     }
     /**************stop **********************/
     if((vibration_state==0)&&(device_speed<2)){
      vehicle_status=3;
     }
      
     
  get_time();
  
  url="http://192.168.0.104:3000/VTS?lat=" + String(current_lattitude,6)+ "&&long=" + String(current_longitude,6)+ "&&bound=" +String(boundry)+ "&&state=" +String(vehicle_status);

  Serial.println(url);
  delay(DELAY_100);
  GSMSerial.println("AT");
  delay(DELAY_100);
  GSMSerial.println("AT+SAPBR=3,1,Contype,GPRS");
  delay(DELAY_100);
  GSMSerial.println("AT+SAPBR=3,1,APN," + apn);
  delay(DELAY_100);
  GSMSerial.println("AT+SAPBR =1,1");
  //runsl();
  delay(DELAY_100);
  GSMSerial.println("AT+SAPBR=2,1");
  delay(DELAY_2000);
  GSMSerial.println("AT+HTTPINIT");
  delay(DELAY_100);
  GSMSerial.println("AT+HTTPPARA=CID,1");
  delay(250);
  GSMSerial.println("AT+HTTPPARA=URL," + url);
  delay(DELAY_200);
  GSMSerial.println("AT+HTTPACTION=0");
  delay(DELAY_5000);
  GSMSerial.println("AT+HTTPREAD");
  delay(DELAY_100);
  GSMSerial.println("AT+HTTPTERM");
   /*****************************check http is available or not********************************************/ 
    dataMessage ="{\"lat\""":"+slash+String(current_lattitude,6)+slash+",\"long\""":"+slash+String(current_longitude,6)+slash+",\"boundry\""":"+slash+String(boundry)+slash+",\"status\""":"+slash+String(vehicle_status)+slash+",\"ts\""":"+slash+String(H)+":"+String(M)+":"+String(S)+slash+"},";
                           
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
  //  }
  }
  
}
return vehicle_status;
} 
/*********************************************************************
Function name : check_message

Description   : To handle Json substrings acquired form JsonParseing function.

Parameters    : Executes commands based on input and sends response Json string.
*********************************************************************/

  int check_message(char *incoming_message) {

  Serial.println("************action***********");
  Serial.println(incoming_message);//CHECK FOR NULL
/******************************parse json action********************************************/
  json_message = json_parser(incoming_message);
  int action = json_message["action"];
  String change_time = json_message["descp"];
  String json_communication_number = json_message["cnum"];
  String input_lattitude= json_message["lattitude"];
  String input_longitude= json_message["longitude"];
  String input_radius= json_message["radius"];
/******************************parse json timer********************************************/
  String timer = json_message["TU"];
  StoreInfo("timer_data", timer);

  switch (action)
  {
    /***************************************To store communication number******************************************/
    case 100:
            StoreInfo("communication_number",json_communication_number);
            delay(10);
            communication_number= RetrieveInfo("communication_number");;
            delay(10);
            get_time();
            response_message="${\"action\":\"200\",\"deviceId\":\""+String(Did)+"\",\"ts\":\""+String(TS)+"\"}$";
            sms_gsm(response_message);
            break;

 /****************************************get all information and store in eeprom****************************************************************************/  
   case 101:{
    
    /************************EEPROM WRITE OF LATTITUDE*****************************************/

            StoreInfo("home_lattitude",input_lattitude);
            delay(10);
                                       
  /*********************EEPROM WRITE OF LONGITUDE*************************/
            StoreInfo("home_longitude",input_longitude);
            delay(10);
/*********************EEPROM WRITE OF radius*************************/
            StoreInfo("RADIUS",input_radius);
            delay(10);
/*****************************************************read id*******************************/
            string_lattitude= RetrieveInfo("home_lattitude");
            string_longitude= RetrieveInfo("home_longitude");                                         
  
             Serial.println("the LATTITUDE is");
             Serial.println(string_lattitude);
 
             Serial.println("the LONGITUDE  id iiiiiiiis");
             Serial.println(string_longitude);

             Serial.println("the Radius iiiiiiiis");
             Serial.println(radius);
/**************************SEND RESPONSE***************************************/
           // homeLat=(JsonData2.toFloat());
            get_time();
           response_message="${\"resp\":\"101\",\"deviceId\":\""+String(Did)+"\",\"State\":\""+String(current_lattitude,6)+"\",\"Hum\":\""+String(current_longitude,6)+"\",\"Temp\":\""+String(boundry)+"\",\"status\":\""+String(vehicle_status)+"\"}}$";
           sms_gsm(response_message);
  }
            break;
     case 102:/**/
             {

             /*********************split alarm timer*********************************/
                         
              int got_alarm_time = DecodeTimers(timer);

        if (got_alarm_time != ERROR_CASE) {
          Serial.println("Error Timer json");
        }

        if (got_alarm_time == 1) {
          StoreInfo("RepeatData", "0");
          repeat_mode = 0;
          check_alarm_timer(10, repeat);
         
        }
              get_time();                
            response_message="${\"resp\":\"102\",\"deviceId\":\""+String(Did)+"\",\"State\":\""+String(current_lattitude,6)+"\",\"Hum\":\""+String(current_longitude,6)+"\",\"Temp\":\""+String(boundry)+"\",\"status\":\""+String(vehicle_status)+"\"}}$";
            sms_gsm(response_message);
                
             }     
                  break;
    case 111: {

        rtc_update(change_time);
        got_updated_time = 1;

           }
           break;
    
  default:{
      Serial.println("wrong");
    }
  break;
}
 return action;
}
/*******************************************send_bluetooth_input******************************
     FUNCTION: send_bluetooth_input

     DISCRIPTION:Takes output string from bluetooth and does json parsing and gives the
     ssid and password to wifi connection function for connection

     PARAMETERS: int

     RETURNS:   bluetooth input status
**********************************************************************************/
int send_bluetooth_input(String bluetooth_string) {
/**************************the input string is a null string***********************************************************/
int int_bluetooth=-1;
  if (bluetooth_string == "") {
    Serial.println("input null string from bluetooth");
    int_bluetooth = ERROR_CASE;
  }

 if (bluetooth_string != "") {
     int_bluetooth = 1;
 /*****************************************************send all*****************************************/
    //if (readString=="\"{\"ssid\":\"\",\"password\":\"1\"}\""){
    if (bluetooth_string=="1"){
        url="$kkkkkkkkkkkkkkkkkkkk$";
        GSMSerial.println("AT+CNMI=2,2,0,0,0");
         delay(DELAY_1000);
        GSMSerial.println("AT+CMGF=1");
        delay(DELAY_500);
        GSMSerial.println("AT+CMGS=\"+91"+String(Rnum)+"\"\r"); 
        delay(DELAY_1000);
        GSMSerial.println(url);
        delay(DELAY_100);
        GSMSerial.println((char)26);
        delay(DELAY_1000);
        GSMSerial.println("AT+CMGD=1,4");
        delay(DELAY_100);
        url="";
        dataMessage ="{\"lat\""":"+slash+String(current_lattitude,6)+slash+",\"long\""":"+slash+String(current_longitude,6)+slash+",\"boundry\""":"+slash+String(boundry)+slash+",\"status\""":"+slash+String(vehicle_status)+slash+",\"ts\""":"+slash+String(H)+":"+String(M)+":"+String(S)+slash+"}]}";
                        
        Serial.print("Save data: ");
        Serial.println(dataMessage);
        appendFile(SD, "/data.txt", dataMessage.c_str());
        readFile(SD, "/data.txt");
        writeFile(SD, "/data.txt", "{\"DEVICEID\":\"123\",\"events\":[\r\n");
        int_bluetooth = 2;
        }
   /*****************************************************delete all*****************************************/
        //  if (readString=="\"{\"ssid\":\"\",\"password\":\"0\"}\""){
        if (bluetooth_string=="0"){
             Serial.print("delete from sd card");
              writeFile(SD, "/data.txt", "\r\n");
              int_bluetooth = 0;
              }

}
bluetooth_string="";
return int_bluetooth;
}
int sms_gsm(String response_message){
      
    GSMSerial.println("AT+CNMI=2,2,0,0,0");
    delay(DELAY_1000);
    GSMSerial.println("AT+CMGF=1");
    delay(DELAY_500);
    GSMSerial.println("AT+CMGS=\"+91"+String(Rnum)+"\"\r"); 
    delay(DELAY_1000);
    GSMSerial.println(response_message);
    delay(DELAY_100);
    GSMSerial.println((char)26);
    delay(DELAY_1000);
    GSMSerial.println("AT+CMGD=1,4");
    delay(DELAY_100);
    response_message="";
  
}
 
