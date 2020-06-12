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
#include "RTClib.h"
#include "String.h"
#include "FS.h"
#include "SD.h"
#include<Wire.h>
#include "BluetoothSerial.h"
#define DRUM1 34
#define THRESHOLD 120
/***********Instances************/
RTC_DS3231 rtc;
TinyGPSPlus gps;
BluetoothSerial ESP_BT; //Object for Bluetooth
HardwareSerial GPSSerial(2);
HardwareSerial GSMSerial(1);
/*********Pin Assignment***********/
int sense_Pin=A0;
int vibr_pin=10;
int LED_Pin=13;
int L=8;
int T=12;
int  itration11,itration4,itration10=0;
byte val = 0;
/*******Global Variables********/
const int MPU=0x68;
int client_length;
char clientt[33];
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265;
int maxVal=402;
String qw,ww,str_Timer1_hr_off,str_Timer1_mins_off;
unsigned long ddd=0;
unsigned long ddd1=0;
int r11=0;
int sp=1;
int ip=0;
int ij,ilk,il=0;
int ro=0;
int i1=0;
int rrK;
int rrK1;
int rrK2;
int CommFlag=0;
int p111=0,x111=0;
float dist=0;
float thresholdDistance;
long int runtime,H55=0,P55=0,v55=0,t1=0;
int k55=0;
float altitu;
String y,m,d,h,mi, s;
String readString;
String string_radius;
String str12K;
String str13K1;
String str12K1;
String str13K2;
String str12K2;
String  str13K;
String Date1,Date2,Date3;
int Repeat1;  
String  H,M,S;
unsigned long time_now12=0; 
unsigned long time_now1=0; 
unsigned long time_now12k=0; 
unsigned long time_now1k=0; 
int we=2;
int ipp=0;
int did1=0;
int did=0;
int l;
unsigned long  dd11=0;
long int D111=0;
int state=0;
int TransCmpt=0;
int i=0;
int prev=0;
int r111=0;
float lat, lon;
unsigned long  u1=0;
long int yo=0;
float swe,kmp,kmp11=0,prevkmp=0;
int qwest,qwe=0;
String url="";
char IncomingMessage[500];
String inString = "";  
int boundry=0;
String slash="\"";
int b=0;
int n=0;
int v=0;
int dp=0;
int u;
int radius_length;
int Iteration1=0;
int q=0;
//double p;
//double o;
double angley;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
volatile byte counter=0;
char str[15];
char str1[15];
char str4[15];
//unsigned long int d=micros();
boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan
String DOW;
String str_Timer1_hr,str_Timer1_mins,str_Timer2_hr,str_Timer2_mins,str_Timer3_hr,str_Timer3_mins="";
String urll="";
int Iteration2,j=0;
char str2[15];
char str3[15];
char radius[3];
volatile byte count;
unsigned int rpm=0;
unsigned int z11=0;
unsigned long prevrpm;
unsigned long duration;
unsigned long prevmillis;
unsigned long lastmillis;
unsigned long time_now = 0;
unsigned long time_now123=0;
int w11,sethour,setdate,setmonth,setyear,setmin,setsec=0;
int ClearVariable;
String Timer1,Timer2,Timer3="";
String JsonID1="";
String JsonData1="";
String JsonID2="";
String JsonData2="";
String JsonID3="";
String JsonData3="";
String JsonID4="";
String JsonData4="";
String JsonID5="";
String JsonData5="";
String JsonID6="";
String JsonData6="";
String TS="";
String RespMsg="";
String Did="VTSGID001";
String Cnum="0000000000";
String Dsnum="6366616417";
String Rnum="8220480192";
//String Rnum="9731031493";
int clength;
  char  clienttu[9];
/****************GPS Variables*********************/
 float latitude , longitude;
  int year , month , date, hour , minute , second;
  String date_str , time_str , lat_str , lng_str;
  int pm,sd_write_status;
  /*************************SD CARD*****************************/
  uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// Sleep for 10 minutes = 600 seconds
uint64_t TIME_TO_SLEEP = 600;

// Define CS pin for the SD card module
#define SD_CS 5
RTC_DATA_ATTR int readingID = 0;
String dataMessage;
#define ONE_WIRE_BUS 21
float temperature;
String formattedDate;
String dayStamp;
String timeStamp;
/*************void functions***************/
void GetTime(void);
void JsonParsing(void);
String ReadFromEeprom(char add);
void WriteToEeprom(char add,String data);
void isr1(void);
void GetCtrlTime(void);
void serialEvent(void);
void TimerAction(char *IncomingMessage);
/***************************************/
//float  homeLat = 12.932635;
 //float homeLon = 77.586454;
 float  homeLat;
 float homeLon;
//float p= 12.932703018188;
//float o = 77.586242675781;
float p;
float o;
/*******************************************/
String JsonID11,JsonID22,JsonID33,JsonID44,JsonID55,JsonID66="";
String JsonData11,JsonData22,JsonData33,JsonData44,JsonData55,JsonData66="";
String ListJsonID1,ListJsonID2,ListJsonID3,ListJsonID4,ListJsonID5,ListJsonID6="";
String ListJsonData1,ListJsonData2,ListJsonData3,ListJsonData4,ListJsonData5,ListJsonData6="";
void setup()
{
    
  /***************SERIAL BEGIN***********************/
  Serial.begin(9600);
    GPSSerial.begin(9600, SERIAL_8N1, 16, 17);
    GSMSerial.begin(9600, SERIAL_8N1,4, 2, false);
  /***********************************eeprom initialization and read data*******************************/
    EEPROM.begin(512); 
    /*********************************write client id**********************************************************/
JsonData2="0000000000";
//JsonData2="afc45d90-3b6c-11ea-b4fe-81l74";
                                  Serial.println(JsonData2);
                                       EEPROM.write(111,JsonData2.length());
                                         EEPROM.commit();
                                                   client_length=EEPROM.read(111);
                                                   Serial.println(client_length);
 
                                    for(itration4=0;itration4<client_length;itration4++)            //loop upto string lenght www.length() returns length of string
                                      {
                                      EEPROM.write(22+itration4,JsonData2[itration4]);                 //Write one by one with starting address of 60
   
                                      }
                                      EEPROM.commit();

 
/*****************************************************read id*******************************
                                                     client_length=EEPROM.read(11);
                                                   Serial.println(client_length);
  itration11=0;
  for( itration10=200;itration10<200+client_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    clientt[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  
Serial.println("the client id is");
  Serial.println(clientt);
delay(100);
     /***************SD CARDDD***********************/
     ESP_BT.begin("Vehicle_tracking"); //Name of your Bluetooth Signal
  // Initialize SD card
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
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


  //logSDCard();
  //readingID++; 
 /***************FILE DELETE***********************/
 
 // deleteFile(SD, "/data.txt");
 /***************PINMODE DECLARATION***********************/
 //pinMode(vibr_pin,INPUT);
 // pinMode(LED_Pin,OUTPUT);
  pinMode(L,OUTPUT);
   // pinMode(T,OUTPUT);
    // pinMode(L,INPUT);    
  //pinMode(7,OUTPUT);
  /***************EEPROM***********************/
// dd11=EEPROM.read(5);
    
/*********************RTC BEGIN*********************/

if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      delay(15000);   
  //Did=ReadFromEeprom(0);                                                          /*Read DeviceID from controller memory*/
  delay(10);
  //Cnum=ReadFromEeprom(22);                                                        /*Read Communication number from controller memory*/
  delay(10);
 // Rnum=ReadFromEeprom(33);                                                        /*Read Register number from controller memory*/
  
  delay(100);
  GetCtrlTime();
 Serial.println(Cnum);
  //Serial.println(Timer2);
  //Serial.println(Timer3);
  //Serial.println(Timer4);
  //Serial.println(Timer5);
  delay(100);
  //DecodeTimers();

 // if(Cnum=="0000000000")                                                         /*Start-up sequence when comm number is not available(Request comm. center for comm number)*/
 // {
   // GetTime();
    
   
//  }
                                                                           /*Start-up sequence when communication number is available*/
  /**else {Wait for GSM module to catch signal
GetTime();
    /*********************CONNECTION HI MESSAGE*********************
 RespMsg="${\"action\":\"REG_DEVICE\",\"deviceId\":\""+String(Did)+"}$";
    GSMSerial.println("AT+CNMI=2,2,0,0,0");
    delay(1000);
    GSMSerial.println("AT+CMGF=1");
    delay(500);
    GSMSerial.println("AT+CMGS=\"+91"+String(Rnum)+"\"\r"); 
    delay(1000);
    GSMSerial.println(RespMsg);
    delay(100);
    GSMSerial.println((char)26);
    delay(1000);
    GSMSerial.println("AT+CMGD=1,4");
    delay(100);
    RespMsg="";
    GetCtrlTime();
     Serial.println("Timer1 and timer2");
              Serial.println(Timer1);
              Serial.println(Timer2);
              Serial.println(Timer3);
              Serial.println(Date1);
              Serial.println(Date1);
              Serial.println(Date3);
              delay(100);
              DecodeTimers();**/
  
 }
 void loop()
 {
  /*******************************Geofence******************************/ 

       while (GPSSerial.available() > 0) {
    char c = GPSSerial.read();
    gps.encode(c);
    if (gps.location.isUpdated()) {
        p=(gps.location.lat());
      Serial.print(p,6);    
    o=(gps.location.lng());
      Serial.print(o,6);  
      Serial.println("altitu");
      Serial.println(gps.altitude.meters());
      Serial.println("speed");
      qwe=(gps.speed.kmph());
      Serial.println(qwe);


    }
  }
     
  
delay(1000);
                                   client_length=EEPROM.read(11);
                                                   Serial.println(client_length);
  itration11=0;
  for( itration10=55;itration10<55+client_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    clientt[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  
Serial.println("the LATTITUDE is");
  Serial.println(clientt);
  /************************/
  clength=EEPROM.read(12);
                                                 Serial.println(clength);
  itration11=0;
  for( itration10=200;itration10<200+client_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    clienttu[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  
Serial.println("the client id iiiiiiiis");

  Serial.println(clientt);
   Serial.println(clienttu);


 qw=String(clientt);
  ww=String(clienttu);
homeLat=qw.toFloat();
    homeLon=ww.toFloat();
  
      Serial.println("o and p is");
   Serial.println(p,6);
   Serial.println(o,6);
      Serial.println("homeLat and long iss");
   Serial.println(homeLat,6);
   Serial.println(homeLon,6);
    dist = HaverSine(homeLat,homeLon,p,o);
    Serial.println("dist:");
   Serial.println(dist);
  checkposition();
    if(TransCmpt==1)                                                            /*Check if message has been received*/
  {
    TransCmpt=0;
    Serial.println("go to pharse");
    JsonParsing();
    
    for(ClearVariable=0;ClearVariable<500;ClearVariable++)                       /*Clear array for next message from comm. center*/
    {
      IncomingMessage[ClearVariable]='\0';
    }
     
    CheckMessage();
  }
                                                   /*Turn on comm. LED*/
 serialEvent();
      stopped();  
      GetTime();
   CheckTimer();
   /**********************************BLUETOOTH******************************************/
    while (Serial.available()) {
    delay(10);                                            //small delay to allow input buffer to fill

    char c = Serial.read();                              //gets one byte from serial buffer
    //breaks out of capture loop to print readstring
    readString += c;
  }                                                      //makes the string readString 

  if (readString.length() >0) {
    Serial.println(readString); //prints string to serial port out
  
  
    Serial.println("the input string from bluetooth is");
      
  }
  /*****************************************************Read the string from the bluetooth**************************************/   
        while (ESP_BT.available()) {
    delay(10);                                            //small delay to allow input buffer to fill

    char c = ESP_BT.read();                              //gets one byte from serial buffer
    //breaks out of capture loop to print readstring
    readString += c;
  }                                                      //makes the string readString 

  if (readString.length() >0) {
    Serial.println(readString); //prints string to serial port out
  
  
    Serial.println("the input string from bluetooth is");
      
    
    
 /*****************************************************JSON pharsing*****************************************/
 delay(99);

   
    int n = readString.length();
       
  
  delay(20);
 /*****************************************************send all*****************************************/
//if (readString=="\"{\"ssid\":\"\",\"password\":\"1\"}\""){
  if (readString=="1"){
    url="$kkkkkkkkkkkkkkkkkkkk$";
    GSMSerial.println("AT+CNMI=2,2,0,0,0");
    delay(1000);
    GSMSerial.println("AT+CMGF=1");
    delay(500);
    GSMSerial.println("AT+CMGS=\"+91"+String(Rnum)+"\"\r"); 
    delay(1000);
    GSMSerial.println(url);
    delay(100);
    GSMSerial.println((char)26);
    delay(1000);
    GSMSerial.println("AT+CMGD=1,4");
    delay(100);
    url="";
    dataMessage ="{\"lat\""":"+slash+String(p,6)+slash+",\"long\""":"+slash+String(o,6)+slash+",\"boundry\""":"+slash+String(boundry)+slash+",\"status\""":"+slash+String(b)+slash+",\"ts\""":"+slash+String(H)+":"+String(M)+":"+String(S)+slash+"}]}";
                        
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
 readFile(SD, "/data.txt");
 writeFile(SD, "/data.txt", "{\"DEVICEID\":\"123\",\"events\":[\r\n");
// Function to get temperature

  }
   /*****************************************************delete all*****************************************/
//  if (readString=="\"{\"ssid\":\"\",\"password\":\"0\"}\""){
if (readString=="0"){
Serial.print("Save data: hgjhgjhggggggggggggggggggg ");
// Function to get temperature
 writeFile(SD, "/data.txt", "\r\n");
  }
  readString="";
  }
 /****************************************end of bluetooth***************************************************/   
  }
 
/*********************************************************************
Function name : serialEvent

Description   : To handle incoming uart communication form GSM module.

Parameters    : Returns Json string acquired form GSM text message.
*********************************************************************/

void serialEvent()
{
  while(GSMSerial.available())
  {
    if(GSMSerial.find("#"))
    {GSMSerial.println("got serial event");
      delay(1000);
      while (GSMSerial.available())
      {
        char inChar=GSMSerial.read();
        IncomingMessage[Iteration1++]=inChar;
        if(inChar=='#')
        {
          Iteration1=0;
          TransCmpt=1;
          Serial.println("serial event over");
          Serial.println(IncomingMessage);
          return;
        }
      }
    }
  }
  
}
void check()
{
  
  if(!(strncmp(str,"start",5)))
{
Serial.println("u r right");
 w11=99;
}
}

void stopped(){
   
/*********no speed***********************/ 
 Serial.println("****stoppped");

 delay(100);
 if(qwe<5){
  qwe=0;
  D111++;
  Serial.println(D111);
 }
 /*********speed increased***********************/
 if(qwe>5)
 {
  D111=0;
 }
 /*********no speed and check id IDLE or not***********************/
 if(D111>30)
 {
   /*********vibration is there***********************/
 /**************KEEP VIBRATION CONSTANT **********************/ 
 delay(2000);
 val = analogRead(DRUM1);
Serial.println("**************************************************valuee");
Serial.println(val);
  if(val > THRESHOLD) { Serial.println("**************************************************HARD");

   r111=1; 
}
/*********vibration is NOT there***********************/
 if(val < THRESHOLD) { 
 Serial.println("**************************************************SOFT");
   r111=0;  
   }
 /**************CHECK FOR angle **********************/
 
    if(millis() > time_now123 + 30000){
     time_now123 = millis();
      /**************running **********************/
     if((r111==1)&&(qwe>1)){
      b=2;
     }
     /**************idle **********************/
     if((r111==1)&&(qwe<2)){
      b=1;
     }
     /**************stop **********************/
     if((r111==0)&&(qwe<2)){
      b=3;
     }
      sd_write_status=EEPROM.read(400);
     //if(sd_write_status==1){
GetTime();
str13K2="airtelgprs.com";
//http://192.168.0.157:3000/VTS?lat=12.33&&long=17.6&&bound=30&&state=1
 //url=String(p,6)+ "/" + String(o,6)+ "/" +String(r111)+ "/" +String(qwe)+ "/" +String(b)+"/"+String(1);
url="http://192.168.0.104:3000/VTS?lat=" + String(p,6)+ "&&long=" + String(o,6)+ "&&bound=" +String(boundry)+ "&&state=" +String(b);
//url="http://192.168.0.157:3000/VTS?lat=" + String(12.1111111)+ "&&long=" + String(77.111111)+ "&&bound=" +String(1)+ "&&state=" +String(2);
Serial.println(url);
  delay(100);
GSMSerial.println("AT");
  //Print GSM Status an the Serial Output;
  delay(100);
  GSMSerial.println("AT+SAPBR=3,1,Contype,GPRS");
  delay(100);
  GSMSerial.println("AT+SAPBR=3,1,APN," + str13K2);
  delay(100);
  GSMSerial.println("AT+SAPBR =1,1");
  //runsl();
  delay(100);
  GSMSerial.println("AT+SAPBR=2,1");
  delay(2000);
  GSMSerial.println("AT+HTTPINIT");
  delay(100);
  GSMSerial.println("AT+HTTPPARA=CID,1");
  delay(250);

  GSMSerial.println("AT+HTTPPARA=URL," + url);
delay(200);
  GSMSerial.println("AT+HTTPACTION=0");
 delay(5000);
  GSMSerial.println("AT+HTTPREAD");

  delay(100);
  GSMSerial.println("AT+HTTPTERM");
    dataMessage ="{\"lat\""":"+slash+String(p,6)+slash+",\"long\""":"+slash+String(o,6)+slash+",\"boundry\""":"+slash+String(boundry)+slash+",\"status\""":"+slash+String(b)+slash+",\"ts\""":"+slash+String(H)+":"+String(M)+":"+String(S)+slash+"},";
                           
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
  //  }
  }
  
}
} 
/***********************************************************************
Function name : WriteToEeprom

Description   : To write given data into controller memory.

Parameters    : Address fro storage and data to be stored.
***********************************************************************/

void WriteToEeprom(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   
  EEPROM.commit();
}
 
/***********************************************************************
Function name : ReadFromEeprom

Description   : To read data from controller memory.

Parameters    : Returns data stored in specified address.
***********************************************************************/ 
String ReadFromEeprom(char add)
{
  int i;
  char data[100];
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}
/***********************************************************************
Function name : JsonParsing

Description   : To parse Json string acquired form serialEvent function.

Parameters    : Returns parsed Json sub-strings.
***********************************************************************/

void JsonParsing()
{
  Serial.println("json for loop");
  Serial.println(IncomingMessage);
  int data_length=strlen(IncomingMessage);
  for(Iteration2=0;Iteration2<=data_length;Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID1="";
      for(Iteration2=Iteration2;Iteration2<data_length;Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID1=JsonID1+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=data_length;Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData1="";
      for(Iteration2=Iteration2;Iteration2<data_length;Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData1=JsonData1+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=data_length;Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID2="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;  
        }
      JsonID2=JsonID2+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData2="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData2=JsonData2+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID3="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID3=JsonID3+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData3="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData3=JsonData3+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID4="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID4=JsonID4+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData4="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData4=JsonData4+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID5="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID5=JsonID5+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData5="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData5=JsonData5+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
    for(Iteration2=Iteration2+4;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID6="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID6=JsonID6+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData6="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData6=JsonData6+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  Serial.println(JsonID1);
  Serial.println(JsonData1);  
  Serial.println(JsonID2);
  Serial.println(JsonData2); 
  Serial.println(JsonID3);
  Serial.println(JsonData3);  
  Serial.println(JsonID4);
  Serial.println(JsonData4); 
  Serial.println(JsonID5);
  Serial.println(JsonData5); 
    Serial.println(JsonID6);
  Serial.println(JsonData6);
  TimerAction(IncomingMessage);
}
/*********************************************************************
Function name : CheckMessage

Description   : To handle Json substrings acquired form JsonParseing function.

Parameters    : Executes commands based on input and sends response Json string.
*********************************************************************/

void CheckMessage()
{
  int Action=JsonData1.toInt();
  String Data1=JsonData2;
  String Data2=JsonData3;
  String Data3=JsonData4;
  String Data4=JsonData5;

  switch (Action)
  {
    case 100:/*To store communication number*/
            WriteToEeprom(22, JsonData3);
            delay(10);
            Cnum=ReadFromEeprom(22);
            delay(10);
            GetTime();
            RespMsg="${\"action\":\"200\",\"deviceId\":\""+String(Did)+"\",\"ts\":\""+String(TS)+"\"}$";
            GSMSerial.println("AT+CNMI=2,2,0,0,0");
            delay(1000);
            GSMSerial.println("AT+CMGF=1");
            delay(500);
            GSMSerial.println("AT+CMGS=\"+91"+String(Cnum)+"\"\r"); 
            delay(1000);
            GSMSerial.println(RespMsg);
            delay(100);
            GSMSerial.println((char)26);
            delay(1000);
            GSMSerial.println("AT+CMGD=1,4");
            delay(100);
            RespMsg="";
            break;

   
                       case 101:{
    
    /************************EEPROM WRITE OF LATTITUDE*****************************************/

                                  Serial.println(JsonData3);
                                       EEPROM.write(11,JsonData3.length());
                                         EEPROM.commit();
                                                   client_length=EEPROM.read(11);
                                                   Serial.println(client_length);
 
                                    for(itration4=0;itration4<client_length;itration4++)            //loop upto string lenght www.length() returns length of string
                                      {
                                      EEPROM.write(55+itration4,JsonData3[itration4]);                 //Write one by one with starting address of 60
   
                                      }
                                      EEPROM.commit();
  /*********************EEPROM WRITE OF LONGITUDE*************************/
                                       Serial.println(JsonData4);
                                       EEPROM.write(12,JsonData4.length());
                                         EEPROM.commit();
                                                   clength=EEPROM.read(12);
                                                   Serial.println(clength);
 
                                    for(itration4=0;itration4<clength;itration4++)            //loop upto string lenght www.length() returns length of string
                                      {
                                      EEPROM.write(200+itration4,JsonData4[itration4]);                 //Write one by one with starting address of 60
   
                                      }
                                      EEPROM.commit();
/*********************EEPROM WRITE OF radius*************************/
                                       Serial.println(JsonData5);
                                       EEPROM.write(13,JsonData5.length());
                                         EEPROM.commit();
                                                   radius_length=EEPROM.read(13);
                                                   Serial.println(radius_length);
 
                                    for(itration4=0;itration4<radius_length;itration4++)            //loop upto string lenght www.length() returns length of string
                                      {
                                      EEPROM.write(40+itration4,JsonData5[itration4]);                 //Write one by one with starting address of 60
   
                                      }
                                      EEPROM.commit();

 
/*****************************************************read id*******************************/
                                                     client_length=EEPROM.read(11);
                                                   Serial.println(client_length);
  itration11=0;
  for( itration10=55;itration10<55+client_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    clientt[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  
Serial.println("the LATTITUDE is");
  Serial.println(clientt);
  /************************/
  clength=EEPROM.read(12);
                                                 Serial.println(clength);
  itration11=0;
  for( itration10=200;itration10<200+client_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    clienttu[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  
Serial.println("the LONGITUDE  id iiiiiiiis");
  Serial.println(clienttu);
 //WriteToEeprom(200,JsonData4);
    Serial.println("101...");
     /************read radius************/
  radius_length=EEPROM.read(13);
  Serial.println(radius_length);
  itration11=0;
  for( itration10=40;itration10<40+radius_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    radius[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  Serial.println("the Radius iiiiiiiis");
  Serial.println(radius);
    DateTime now = rtc.now();
               y=String(now.year(), DEC);
               m=String(now.month(), DEC);
               d=String(now.day(), DEC);
               h=String(JsonData6[0])+String(JsonData6[1]);
               mi=String(JsonData6[3])+String(JsonData6[4]);
               s=String(now.second(), DEC);

              setyear=y.toInt();
               setmonth=m.toInt();
               setdate=d.toInt();
              sethour=h.toInt();
               setmin=mi.toInt();
               setsec=s.toInt();
              
              h=mi="";
             Serial.println("RTC SET");
              Serial.println(JsonData6);
              rtc.adjust(DateTime(setyear, setmonth, setdate, sethour, setmin, setsec));
               
  /**************************SEND RESPONSE***************************************/
            homeLat=(JsonData2.toFloat());
            GetTime();
           RespMsg="${\"resp\":\"101\",\"deviceId\":\""+String(Did)+"\",\"State\":\""+String(p,6)+"\",\"Hum\":\""+String(o,6)+"\",\"Temp\":\""+String(boundry)+"\",\"status\":\""+String(b)+"\"}}$";
            GSMSerial.println("AT+CNMI=2,2,0,0,0");
             delay(1000);
             GSMSerial.println("AT+CMGF=1");
             delay(500);
              GSMSerial.println("AT+CMGS=\"+918220480192\"\r"); 
    delay(1000);
    GSMSerial.println(RespMsg);
    delay(100);
    GSMSerial.println((char)26);
    delay(1000);
    GSMSerial.println("AT+CMGD=1,4");
    delay(100);
    RespMsg="";
  }
            break;
     case 102:/**/
             {  Serial.println(IncomingMessage);
              TimerAction(IncomingMessage);
              GetTime();
              int JsonData33_length = JsonData33.length(); 
                           char list[JsonData33_length + 1];
                           strcpy(list, JsonData33.c_str()); 
                           delay(999);
                           /*********************first date and time*********************************/
                         Serial.println(list);
                          SplitFullTime(list);
              
             //  Timer1_flag=EEPROM.read(411);
//Timer2_flag=EEPROM.read(412);
//Timer3_flag=EEPROM.read(413);
//Timer4_flag=EEPROM.read(414);
//Timer5_flag=EEPROM.read(415);
              
                      
                 
            RespMsg="${\"resp\":\"102\",\"deviceId\":\""+String(Did)+"\",\"State\":\""+String(p,6)+"\",\"Hum\":\""+String(o,6)+"\",\"Temp\":\""+String(boundry)+"\",\"status\":\""+String(b)+"\"}}$";
            GSMSerial.println("AT+CNMI=2,2,0,0,0");
             delay(1000);
             GSMSerial.println("AT+CMGF=1");
             delay(500);
              GSMSerial.println("AT+CMGS=\"+918220480192\"\r"); 
              delay(1000);
              GSMSerial.println(RespMsg);
              delay(100);
               GSMSerial.println((char)26);
               delay(1000);
               GSMSerial.println("AT+CMGD=1,4");
               delay(100);
               RespMsg=Timer1=Timer2=Timer3=Date1=Date2=Date3=""; 
                
             }     
                  break;

    default:{
      Serial.println("wrong");
    }
  break;
}
 return;
}
/***********************************************************************
Function name : SplitFullTime
Description   : To split incoming Json timesheet into seperate times.
Parameters    : Splits and assigns to variables.
***********************************************************************/ 
void SplitFullTime(String fulltime)
{ String T1,T2,T3;

  Date1=Date1+fulltime[8]+fulltime[9]+fulltime[10]+fulltime[11]+fulltime[12]+fulltime[13]+fulltime[14]+fulltime[15];
    T1=T1+fulltime[26]+fulltime[27]+fulltime[28]+fulltime[29]+fulltime[30]+fulltime[31]+fulltime[32];

  Date2=Date2+fulltime[46]+fulltime[47]+fulltime[48]+fulltime[49]+fulltime[50]+fulltime[51]+fulltime[52]+fulltime[53];
  T2=T2+fulltime[64]+fulltime[64]+fulltime[65]+fulltime[66]+fulltime[67]+fulltime[68]+fulltime[69]+fulltime[70];

  Date3=Date3+fulltime[84]+fulltime[85]+fulltime[86]+fulltime[87]+fulltime[88]+fulltime[89]+fulltime[90]+fulltime[91];
  T3=T3+fulltime[102]+fulltime[103]+fulltime[104]+fulltime[105]+fulltime[106]+fulltime[107]+fulltime[108];
  
  Timer1=T1;
  Timer2=T2;
  Timer3=T3;

  T1=T2=T3="";
  
    Serial.println("AFTER SPLITTING");
  Serial.println(Timer1);
   Serial.println(Timer2);
   Serial.println(Timer3);
   Serial.println(Date1);
   Serial.println(Date2);
   Serial.println(Date3);
   WriteToEeprom(50, Timer1);
              delay(10);
              WriteToEeprom(59, Timer2);
              
              delay(10);
              WriteToEeprom(68, Timer3);
              delay(10);
              WriteToEeprom(77, Date1);
              delay(10);
              WriteToEeprom(86, Date2);
              
              delay(10);
              WriteToEeprom(277, Date3);
              delay(10);
              GetCtrlTime();
              Serial.println(Timer1);
              Serial.println(Timer2);
              Serial.println(Timer3);
              Serial.println(Date1);
              Serial.println(Date2);
              Serial.println(Date3);
              delay(100);
DecodeTimers();
}
/***********************************************************************
Function name : DecodeTimers
Description   : To split 5 timers into individual times and flags.
Parameters    : Returns split data of all 5 timers.
***********************************************************************/ 
void DecodeTimers()
{
  str_Timer1_hr="";
  str_Timer1_mins="";
   str_Timer1_hr_off="";
  str_Timer1_mins_off="";

  str_Timer1_hr=str_Timer1_hr+Timer1[0]+Timer1[1];
  str_Timer1_mins=str_Timer1_mins+Timer1[2]+Timer1[3];
   str_Timer1_hr_off= str_Timer1_hr_off+Timer1[4]+Timer1[5];
   str_Timer1_mins_off=str_Timer1_mins_off+Timer1[6]+Timer1[7];
  delay(50); 

  Serial.println(" decode timer str_Timer1_hr");
  Serial.println(str_Timer1_hr);
  Serial.println(str_Timer1_mins);
  Serial.println(str_Timer1_hr_off);
  Serial.println(str_Timer1_mins_off);
  
  str_Timer2_hr="";
  str_Timer2_mins="";
  str_Timer2_hr=str_Timer2_hr+Timer2[0]+Timer2[1];
  str_Timer2_mins=str_Timer2_mins+Timer2[2]+Timer2[3];

  delay(50);
   Serial.println("str_Timer2_hr");
    Serial.println(str_Timer2_hr);
  Serial.println(str_Timer2_mins);
  

  str_Timer3_hr="";
  str_Timer3_mins="";

  str_Timer3_hr=str_Timer3_hr+Timer3[0]+Timer3[1];
  str_Timer3_mins=str_Timer3_mins+Timer3[2]+Timer3[3];
  delay(50); 
     Serial.println("str_Timer2_hr");
    Serial.println(str_Timer3_hr);
  Serial.println(str_Timer3_mins); 
}
void checkposition(){
       /************read radius************/
  radius_length=EEPROM.read(13);
  Serial.println(radius_length);
  itration11=0;
  for( itration10=40;itration10<40+radius_length;itration10++) //loop upto string lenght www.length() returns length of string
  {
    radius[itration11]= EEPROM.read(itration10);
    itration11++;
  }
  Serial.println("the Radius iiiiiiiis");
  Serial.println(radius);
  string_radius=String(radius);

thresholdDistance=string_radius.toFloat();
Serial.println("the Radius iiiiiiiis inside threshold");
  Serial.println(thresholdDistance);
  if((dist>thresholdDistance)&&(p111==0))
 { 
  p111++;
  x111=0;
  digitalWrite(8,HIGH); 
  delay(2000);

Serial.println(p);
  Serial.println(o);
  Serial.println(thresholdDistance);
  Serial.println(dist);
  Serial.println("******************************************************outide boundary");
  boundry=0;
  GetTime();
    dataMessage ="{\"lat\""":"+slash+String(p)+slash+",\"long\""":"+slash+String(o)+slash+",\"boundry\""":"+slash+String(boundry)+slash+",\"status\""":"+slash+String(b)+slash+",\"ts\""":"+slash+String(H)+":"+String(M)+":"+String(S)+slash+"},";
                           
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
  
 }
  if((dist<thresholdDistance)&&(x111==0))
 {
   p111=0;
  x111++;
  boundry=1;
    GetTime();
  dataMessage ="{\"lat\""":"+slash+String(p)+slash+",\"long\""":"+slash+String(o)+slash+",\"boundry\""":"+slash+String(boundry)+slash+",\"status\""":"+slash+String(b)+slash+",\"ts\""":"+slash+String(H)+":"+String(M)+":"+String(S)+slash+"},";
                     
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
  Serial.println("******************************************************inside boundary");
  digitalWrite(8,LOW); 

 }
 }
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

 /*******************************************************************************
Function name : GetTime

Description   : To handle RTC module and get current time and convert to string.

Parameters    : Returns current time as a string.
*******************************************************************************/

void GetTime()
{
DateTime now = rtc.now();
  
  H=String(now.hour(), DEC);
  M=String(now.minute(), DEC);
  S=String(now.second(), DEC);

  char x[3];
  strcpy(x,M.c_str());
  int y=atoi(x);
  if(y<10)
  {
    M="";
    M=String(0)+String(now.minute(), DEC);
  }
  char xx[3];
  strcpy(xx,H.c_str());
  int yy=atoi(xx);
  if(yy<10)
  {
    H="";
    H=String(0)+String(now.hour(), DEC);
  }
  
  TS=String(H)+":"+String(M)+":"+String(S);
  Serial.println(TS);
  DOW=daysOfTheWeek[now.dayOfTheWeek()];
}
// Write the sensor readings on the SD card
void logSDCard() {
  dataMessage = String(readingID) + "," + String(dayStamp) + "," + String(timeStamp) + "," + 
                String(temperature) + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
void readFile(fs::FS &fs, const char * path){
Serial.printf("Reading file: %s\n", path);
 
File file = fs.open(path);
if(!file){
Serial.println("Failed to open file for reading");
return;
}
delay(1000); 
Serial.print("Read from file: ");
while(file.available()){
//Serial.write(file.read());
 ESP_BT.print(char(file.read()));
 delay(10);
 //ESP_BT.println("rajatammma");
}
}
void deleteFile(fs::FS &fs, const char * path){
Serial.printf("Deleting file: %s\n", path);
if(fs.remove(path)){
Serial.println("File deleted");
} else {
Serial.println("Delete failed");
}
}



void CheckTimer(){
  /** Serial.println(" is str_Timer1_hr");
 Serial.println(str_Timer1_hr);
  Serial.println(" is str_Timer1_hr off");
 Serial.println(str_Timer1_hr_off);
    if((H>=str_Timer1_hr)&&(M>=str_Timer1_mins))
    {
      Serial.println("*************************motor1 oN");
                                                                  
                                                                     EEPROM.write(400,1);
                                                                 EEPROM.commit();
                                                                 Serial.println("strart writting to sd card");
  }
  if((H>=str_Timer1_hr_off)&&(M>=str_Timer1_mins_off)&&(EEPROM.read(400)==1))
    {
     EEPROM.write(400,0);
                                                                 EEPROM.commit();
                                                                 
      Serial.println("*************************motor1 oFF");
    }**/
}
/***********************************************************************
Function name : JsonParsing
Description   : To parse Json string acquired form serialEvent function.
Parameters    : Returns parsed Json sub-strings.
***********************************************************************/
 void TimerAction(char *IncomingMessage){
 for(Iteration2=0;Iteration2<=strlen(IncomingMessage);Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID11="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID11=JsonID11+IncomingMessage[Iteration2+1];
      
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=strlen(IncomingMessage);Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData11="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData11=JsonData11+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=strlen(IncomingMessage);Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID22="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;  
        }
      JsonID22=JsonID22+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData22="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData22=JsonData22+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
  
for(Iteration2=Iteration2+4;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID33="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;
        }
      JsonID33=JsonID33+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='[')
    {
      JsonData33="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]==']')
        {
          break;
        }
      JsonData33=JsonData33+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
   for(Iteration2=Iteration2+4;Iteration2<=strlen(IncomingMessage);Iteration2++)
  {
    if(IncomingMessage[Iteration2]=='"')
    {
      JsonID44="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+1]=='"')
        {
          break;  
        }
      JsonID44=JsonID44+IncomingMessage[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=(strlen(IncomingMessage));Iteration2++)
  {
    if(IncomingMessage[Iteration2]==':')
    {
      JsonData44="";
      for(Iteration2=Iteration2;Iteration2<(strlen(IncomingMessage));Iteration2++)
      {
        if(IncomingMessage[Iteration2+2]=='"')
        {
          break;
        }
      JsonData44=JsonData44+IncomingMessage[Iteration2+2];
      }
    break;
    }
  }
Serial.println("JsonID11");
Serial.println(JsonID11);
Serial.println(JsonData11);
Serial.println(JsonID22);
Serial.println(JsonData22);
Serial.println(JsonID33);
Serial.println(JsonData33);
Serial.println(JsonID44);
Serial.println(JsonData44);
}
void listAction(char *list){
 for(Iteration2=0;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]=='"')
    {
      ListJsonID1="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='"')
        {
          break;
        }
      ListJsonID1=ListJsonID1+list[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]==':')
    {
      ListJsonData1="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+2]=='"')
        {
          break;
        }
      ListJsonData1=ListJsonData1+list[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]=='"')
    {
      ListJsonID2="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='"')
        {
          break;  
        }
      ListJsonID2=ListJsonID2+list[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+1;Iteration2<=(strlen(list));Iteration2++)
  {
    if(list[Iteration2]==':')
    {
      ListJsonData2="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='}')
        {
          break;
        }
      ListJsonData2=ListJsonData2+list[Iteration2+1];
      }
    break;
    }
  }
  for(Iteration2=Iteration2+2;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]=='"')
    {
      ListJsonID3="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='"')
        {
          break;
        }
      ListJsonID3=ListJsonID3+list[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]==':')
    {
      ListJsonData3="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+2]=='"')
        {
          break;
        }
      ListJsonData3=ListJsonData3+list[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]=='"')
    {
      ListJsonID4="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='"')
        {
          break;  
        }
      ListJsonID4=ListJsonID4+list[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+1;Iteration2<=(strlen(list));Iteration2++)
  {
    if(list[Iteration2]==':')
    {
      ListJsonData4="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='}')
        {
          break;
        }
      ListJsonData4=ListJsonData4+list[Iteration2+1];
      }
    break;
    }
  }
  for(Iteration2=Iteration2+2;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]=='"')
    {
      ListJsonID5="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='"')
        {
          break;
        }
      ListJsonID5=ListJsonID5+list[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+2;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]==':')
    {
      ListJsonData5="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+2]=='"')
        {
          break;
        }
      ListJsonData5=ListJsonData5+list[Iteration2+2];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+4;Iteration2<=strlen(list);Iteration2++)
  {
    if(list[Iteration2]=='"')
    {
      ListJsonID6="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='"')
        {
          break;  
        }
      ListJsonID6=ListJsonID6+list[Iteration2+1];
      }
    break;
    }
  }
  
  for(Iteration2=Iteration2+1;Iteration2<=(strlen(list));Iteration2++)
  {
    if(list[Iteration2]==':')
    {
      ListJsonData6="";
      for(Iteration2=Iteration2;Iteration2<(strlen(list));Iteration2++)
      {
        if(list[Iteration2+1]=='}')
        {
          break;
        }
      ListJsonData6=ListJsonData6+list[Iteration2+1];
      }
    break;
    }
  }
  Serial.println(ListJsonID1);
 Serial.println(ListJsonData1);
 Serial.println(ListJsonID2);
 Serial.println(ListJsonData2);
   Serial.println(ListJsonID3);
 Serial.println(ListJsonData3);
 Serial.println(ListJsonID4);
 Serial.println(ListJsonData4);
  Serial.println(ListJsonID5);
 Serial.println(ListJsonData5);
 Serial.println(ListJsonID6);
 Serial.println(ListJsonData6);
}
/***********************************************************************
Function name : GetCtrlTime

Description   : To read timesheet from controller memory.

Parameters    : Returns timesheet from eeprom.
***********************************************************************/ 
void GetCtrlTime()
{
    Timer1= ReadFromEeprom(50);
    Timer2= ReadFromEeprom(59);
    Timer3= ReadFromEeprom(68);
    Date1= ReadFromEeprom(77);
    Date2= ReadFromEeprom(86);
    Date3= ReadFromEeprom(277);
    Serial.println(" get vtrl timer Timer1");
    Serial.println(Timer1);
    Serial.println(Timer2);
    Serial.println(Timer3);
    Serial.println(Date1);
    Serial.println(Date2);
    Serial.println(Date3);
    delay(10);
}
