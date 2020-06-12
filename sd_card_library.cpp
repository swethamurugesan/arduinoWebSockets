/**************************************************************************************
 * 
 * Name          : geofence_libray.cpp
 * 
 * Version       : 1.0
 * 
 * Author        : Raji
 * 
 * Description   : This library is to connect bluetooth module  with nodemcu12e
 *                  in order to connect to wifi and ip address by sending wifi 
 *                  credentials via bluetooth of SWMS          
 *  
 *  WiFi Input        : {"ssid":"yyyyy","password":"xxxx"}              
 * 
 * Pin Discription: (bluetooth->nodemcu12E) 
 *                           Rx->Tx  
 *                           Tx->Rx
 *                           Vcc->Vcc
 *                           Gnd->Gnd.          
 **************************************************************************************/
  #include "sd_card_library.h"
  #include "Arduino.h"
  #include "FS.h"
  #include "SD.h"
  #include<Wire.h>
  #include <String.h>
  /********************************#define************************************/
  #define DELAY_10 10
  #define DELAY_100 100
  #define SD_CS 5
  /******declarations************/

   String dataMessage;
 
  /*******************************************action_check***********************************************
   * FUNCTION   :  bluetooth
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
int sd_initialization(void){
   Serial.println("hii im sd card ");
   SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return -1;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return -2;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return -3;    // init failed
  }

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  

  //logSDCard();
  //readingID++; 

  
 }

void logSDCard() {
 // dataMessage = String(readingID) + "," + String(dayStamp) + "," + String(timeStamp) + "," + 
              //  String(temperature) + "\r\n";
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

 /*******************************************action_check***********************************************
   * FUNCTION   :  bluetooth
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
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
 /*******************************************action_check***********************************************
   * FUNCTION   :  bluetooth
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
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
Serial.write(file.read());
// ESP_BT.print(char(file.read()));
 delay(10);
 //ESP_BT.println("rajatammma");
}
}
 /*******************************************action_check***********************************************
   * FUNCTION   :  bluetooth
   * 
   * DISCRIPTION: This function gets ssid and password from the mobile phone and stores in eeprom 
   *              which is used for connecting with wifi continously even after re-starting the device
   * 
   * PARAMETERS : int
   * 
   * RETURNS    : if connected to wifi returns 11 else 9  
****************************************************************************************************/
void deleteFile(fs::FS &fs, const char * path){
Serial.printf("Deleting file: %s\n", path);
if(fs.remove(path)){
Serial.println("File deleted");
} else {
Serial.println("Delete failed");
}
}


