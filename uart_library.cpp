/**********************************************************
 * Name: UART Library
 
 * Version: 1.0
 
 * Author: Rajat
 
 * Description: Library to handle uart communication 
 *********************************************************/
#include "Arduino.h"
#include "uart_library.h"
void serialEvent(void);
String uartString(void);
int send_to_gsm(String mobile_number,String gsm_string);
int gsm_initialisation(void);
String incoming_message="";
String uart_string="";
int receive_complete;

/*********************************************************************
Function name : serialEvent
Description   : To handle incoming uart communication form GSM module.
Parameters    : Returns Json string acquired form GSM text message.
*********************************************************************/

void serialEvent(void)
{
while(Serial.available())
{
if(Serial.find("$"))
{
delay(1000);
while (Serial.available())
{
  char inChar=Serial.read();
  if(inChar=='$')
{
receive_complete=1;
return;
}

//Serial.println(inChar);
incoming_message=incoming_message+inChar;

}
}
}
}

/***********************************************************************
Function name : uartString
Description   : This function will check whether data received completly or not
Parameters    : returns received uart string
***********************************************************************/ 

String uartString(void)
{
    serialEvent();
    uart_string="";
if(receive_complete==1)
{
 // Serial.println(str);
  uart_string=incoming_message;
  incoming_message="";
  receive_complete=0;
 
}
return uart_string;
}


/***********************************************************************
Function name : check_timer
Description   : To check timer data and control solenoid accordingly.
Parameters    : Solenoid control with notification.
***********************************************************************/ 

int send_to_gsm(String mobile_number,String gsm_string)
{
  Serial.println("AT+CMGS=\"+91" + mobile_number + "\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println(gsm_string);// The SMS text you want to send
  delay(100);
  Serial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
  Serial.println("AT+CMGD=1,4");
  delay(100);
  return 1;
}

/***********************************************************************
Function name : check_timer
Description   : To check timer data and control solenoid accordingly.
Parameters    : Solenoid control with notification.
***********************************************************************/ 

int gsm_initialisation(void)
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(500);
}

