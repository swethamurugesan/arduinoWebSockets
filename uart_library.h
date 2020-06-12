/**********************************************************
 * Name: UART Library
 
 * Version: 1.0
 
 * Author: Rajat
 
 * Description: Library to handle uart communication 
 *********************************************************/ 


#ifndef UARTLIBRARY0_H_
#define UARTLIBRARY0_H_

/*******Libraries********/

extern String uartString(void);
extern int send_to_gsm(String mobile_number,String gsm_string);
extern int gsm_initialisation(void);

#endif /* UARTLIBRARY_H_ */

