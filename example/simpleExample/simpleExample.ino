/* This sketch sends temperature to the serial port. After uploading, open the serial monitor (ctrl+shift+m) to see it.

  created 8 March 2015
  by Koos Looijesteijn
 
  This code is in the public domain.
   
  Before use, the sensor must be calibrated and put in polling mode. Use the cozir.h library for that: https://github.com/kslstn/cozir

*/

#include <SoftwareSerial.h>
#include "Kozir.h"

SoftwareSerial nss(2,3); // Pin 2 = Arduino receiver pin (Rx), connect to sensor transmitter pin (Tx). Pin 3 = Arduino Tx, connect it to sensor Rx.
COZIR czr(nss);

bool waitingForSensor = false;
unsigned long lastPoll = 0;// last poll moment

void setup(){
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis = millis(); // Current time in millis.
  
  // Request temperature, humidity and CO2 every 10 seconds:
  if ((currentMillis - lastPoll > 10000 ) && (waitingForSensor == false)){
    czr.requestCelsius();
    lastPoll = currentMillis;
    waitingForSensor = true; // Don't allow data requests until sensor's buffer has been read.
  }
  else if (waitingForSensor) { // A request has been made, so now we can ask the sensor to return something
    float response = czr.getResponseCelsius();
    if (response != (uint16_t) -1) { // If the response does not equal the non-sensical large number that is returned when the sensor hasn't replied yet ...
      Serial.println("temperature:");
      Serial.println(response); // .. show the temperature ...
      waitingForSensor = false; // ... and allow a new request.
    }
  }
}