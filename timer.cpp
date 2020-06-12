/**************************************************************************************
 * 
 * Name          : basicmodes.cpp
 * 
 * Version       : 1.0
 * 
 * Author        : Raji
 * 
 * Description   : This library is used for basic modes like 
 *                  1.Manual.
 *                  2.Timer.
 *                 we have to include manual library and timer library 
 *                 
 *                          
 **************************************************************************************/
  #include "timer.h"
  #include<EEPROM.h>
  #include <String.h>
  #include <SPI.h> 
  #include "RTClib.h"
  /************************************************************/
  #define DELAY_200 200
  #define DELAY_50  50
  #define DELAY_10  10
 /************************************creating object****************************************/
  RTC_DS3231 rtc;
/*********************void declarations****************************/
  int timer_flag,TimerFlag,off_time,on_time;
  int timer_status,MotorValue;
  String timer_one="";
  String DOW;
/***************timer_one variables**************/
  String str_timer_hr;
  String TS;
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  String str_timer_mins;
  String str_timer;
  String str_timer_flag,str_timer_repeat;
  String str_timer_total_time;
  int  int_hour,int_min,timer_on,repeat_mode,repeat;
/*********timer1 variables*************/
  String rtc_hour,rtc_minutes,rtc_seconds;
/*******************************************************************************
Function name : get_time
Description   : To handle RTC module and get current time and convert to string.
Parameters    : Returns current time as a string.
*******************************************************************************/
int get_time(){
/****************************************RTC IN HOUR:MINUTE:SECONDS***********************************************/
DateTime now =  rtc.now();
  if(165==now.hour()){//MAKE CONATANT
    return -1;
    Serial.println("rtc did not return proper value");
  }  
  
  rtc_hour=String(now.hour(), DEC);
  rtc_minutes=String(now.minute(), DEC);
  rtc_seconds=String(now.second(), DEC);
  Serial.println("/***********time in rtc********************************/");
  TS=String(rtc_hour)+":"+String(rtc_minutes)+":"+String(rtc_seconds);
  Serial.println(TS);
  DOW=daysOfTheWeek[now.dayOfTheWeek()];
  Serial.println(DOW);
  
 return 1;  
}
/*******************************************************************************
Function name : rtc_update
Description   : Get RTC Time and day 
Parameters    : Returns -1 if no rtc module is connected.
*******************************************************************************/
int rtc_update(String change_time){
  Serial.println("rtc get time &&&&&&&&&&&&commands");
  Serial.println(change_time);
   DateTime now = rtc.now();
            if(change_time=="null"){
                Serial.println("rtc_update got null string");
              return -1;
            }
              if(change_time!="null"){
              
              String set_year=String(now.year(), DEC);
              String set_month=String(now.month(), DEC);
              String set_date=String(now.day(), DEC);
              String set_hour=String(change_time[0])+String(change_time[1]);
              String set_minutes=String(change_time[3])+String(change_time[4]);
              String set_seconds=String(now.second(), DEC);

              int int_set_year=set_year.toInt();
              int int_set_month=set_month.toInt();
              int int_set_date=set_date.toInt();
              int int_set_hour=set_hour.toInt();
              int int_set_minutes=set_minutes.toInt();
              int int_set_seconds=set_seconds.toInt();
              
              set_hour=set_minutes="";
              
              Serial.println("RTC SET");
              Serial.println(change_time);
              Serial.println(int_set_hour);
              
              rtc.adjust(DateTime(int_set_year, int_set_month, int_set_date, int_set_hour, int_set_minutes, int_set_seconds));
              return 1;
              }             
              get_time();
}
/***********************************************************************
Function name : DecodeTimers
Description   : To split 5 timers into individual times and flags.
Parameters    : Returns split data of all 5 timers.
***********************************************************************/ 
int DecodeTimers(String timer_one)
{
  if(timer_one==""){
     Serial.println("Error input data for decodetTimer");
    return -1;
  }
  if(timer_one!=""){
    
  /*****************************INITIALIZATION******************************/
  str_timer_hr="";
  str_timer_mins="";
  str_timer_total_time="";
  str_timer_flag="";
  str_timer_repeat="";
   /*****************************split the timers******************************/
  str_timer_hr=str_timer_hr+timer_one[0]+timer_one[1];
  str_timer_mins=str_timer_mins+timer_one[2]+timer_one[3];
  str_timer_total_time=str_timer_total_time+timer_one[4]+timer_one[5];
  str_timer_flag=str_timer_flag+timer_one[6];
  str_timer_repeat=str_timer_repeat+timer_one[7];
  delay(DELAY_50); 
  
  
  
  Serial.println(" in decoder time is str_timer_one_hr");
  Serial.println(str_timer_hr);
  Serial.println(str_timer_mins);
  Serial.println(str_timer_total_time);
  Serial.println(str_timer_flag);
  /***********************************repeat to int ******************************************/
  repeat=str_timer_repeat.toInt();
  /***********************************timer flag to int ******************************************/
  timer_flag=str_timer_flag.toInt();
  /***********************************on time calculation******************************************/
  on_time=((str_timer_hr.toInt())*60)+str_timer_mins.toInt();
  /***********************************off time calculation******************************************/
  off_time=on_time+(str_timer_total_time.toInt());
  get_time();
  Serial.println(DOW);

 Serial.println("ON DECODE TIMER");
  Serial.println(on_time);
  Serial.println("OFF DECODE TIMER");
  Serial.println(off_time);
  return 1;
  }
}

  

int rtc_initialization(void){
    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    return -1;
     }
     
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  return 1;
}

int declarations_initialization(void){
  
}
int check_alarm_timer(int motor,int repeat_mode)
{ 
  
  get_time();
  /******************************************CONVERT PRESENT TIME TO INT********************************************************************/
  int_hour=rtc_hour.toInt();
  int_min=rtc_minutes.toInt();
    /******************************************CONVERT PRESENT TIME TO MINUTES********************************************************************/
  int current_time=(int_hour*60)+int_min;
  Serial.println("*******************current_time");
  Serial.println(current_time);
  Serial.println(on_time);
  Serial.println(off_time);
  Serial.println(timer_flag);
  Serial.println(repeat);
  Serial.println(repeat_mode);
  delay(DELAY_200);
  /*****************************************************TIMER ON***********************************************************/        
  if((on_time<=current_time)&&(current_time<off_time)&&(timer_flag==1)&&(repeat||(!repeat_mode)))               //&&(repeat==1)
  {                                                            
    timer_status=1;
    digitalWrite(motor,HIGH);
    MotorValue = digitalRead(motor);
    
    Serial.println("*******************************motor on");
    delay(DELAY_10);
    timer_flag=0;
  }  
 
/******************************************TIMER OFF************************************************/
  if((current_time>=off_time)&&(1==timer_status))
  {
    timer_status=0;
    digitalWrite(motor,LOW);
    Serial.println(timer_status);
    Serial.println("*******************************motor off");
    delay(DELAY_10);
  }
  else
     timer_status=-1;
  
  return timer_status;

   
}
