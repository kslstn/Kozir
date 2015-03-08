/* This sketch sends values for temperature, humidity and CO2 to the serial port. After uploading, open the serial monitor (ctrl+shift+m) to see it.

  created 8 March 2015
  by Koos Looijesteijn
 
  This code is in the public domain.
   
  Before use, the sensor must be calibrated and put in polling mode. Use the cozir.h library for that: https://github.com/kslstn/cozir

*/

#include <SoftwareSerial.h>
#include "Kozir.h"

SoftwareSerial nss(2,3); // Pin 2 = Arduino receiver pin (Rx), connect to sensor transmitter pin (Tx). Pin 3 = Arduino Tx, connect it to sensor Rx.
COZIR czr(nss);

static bool waitingForSensor = false;
static byte type; // Holds requested type of data: 0 = temperature, 1 = humidity, 2 = CO2
unsigned long lastPoll = 0;// last poll moment

void setup(){
  Serial.begin(9600); 
  delay(5000); // The sensor needs a few seconds to calibrate.
}

void loop(){
  unsigned long currentMillis = millis();
  
  // Request temperature, humidity and CO2 every 10 seconds:
  if ((currentMillis - lastPoll > 10000 ) && (waitingForSensor == false)){

    if (type == 0){
      czr.requestCelsius();
    }
    if (type == 1){
      czr.requestHumidity();      
    }
    if (type == 2) {
      czr.requestCO2();
      lastPoll = currentMillis;
    } 
    waitingForSensor = true; // Don't allow data requests until sensor's buffer has been read.
  }
  else if (waitingForSensor) { // A request has been made, so now we can ask the sensor to return something
    float response;

    if (type == 0){
      response = (float) czr.getResponseCelsius();
      if (response != (uint16_t) -1) {
        Serial.println("temperature:");
      }
    }
    if (type == 1){
      response = (float) czr.getResponseHumidity();
      if (response != (uint16_t) -1) {
        Serial.println("humidity:");
      }
    }
    if (type == 2) {
      response = (float) czr.getResponseCO2();
      if (response != (uint16_t) -1) {
        Serial.println("CO2:");
      }
    }
    if (response != (uint16_t) -1) {
      Serial.println("reset");
      type++;
      if (type > 2){
        type = 0;
      }
      waitingForSensor = false;
      Serial.println(response);
    }
  }
}