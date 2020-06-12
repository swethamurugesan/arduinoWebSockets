#ifndef TIMER_H_
#define TIMER_H_
/*****C LIBRARIES*******/
  #include <String.h>
  #include<EEPROM.h>
  #include "Arduino.h"
/**********************other external declarations*************************/
  extern int total_mins;
  extern int motor, timer_status;;

  extern  String alarm_timer;

  extern  String DOW,TS;
  extern char daysOfTheWeek[7][12];
  extern int got_updated_time,timer_flag;
  extern int repeat;
  extern int timer_flag,in_timer_count;
/*****************external void declarations***************************************/
  extern int get_time(void);
  extern int rtc_update(String change_time);
  extern int DecodeTimers(String timer_one);
  extern int rtc_initialization(void);
  extern int declarations_initialization(void);
  extern int check_alarm_timer(int motor,int repeat_mode);
/*********************external timer declarations********************************/
  extern int on_hour,total_mins,TimerFlag;
  extern int Repeat1,timer_on;
  extern int int_hour,int_min,r1,mins1_on;
  extern int  MotorValue,off_time,on_time,repeat_mode;
  extern String timer_one;
extern String rtc_hour,rtc_minutes,rtc_seconds;
extern String str_timer_hr;
extern String  str_timer_mins;
extern String str_timer_total_time,str_timer_repeat;
extern String str_timer_flag;

#endif
