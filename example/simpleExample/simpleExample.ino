/* This sketch sents values temperature, humidity, CO2 and the digital filter setting to the serial port. After uploading, open the serial monitor (ctrl+shift+m) to see those.

  created 13 Feb 2015
  by Koos Looijesteijn
 
  This code is in the public domain.
   
  Upon first use, the sensor must be calibrated. If you don't get proper readings, do this:
  1) Upload the SetToPolling sketch - Because the library is made for polling.
  2) Move the sensor to an area with fresh air
  3) Upload the Calibrate sketch
  4) Upload this sketch again.
  
  For logging in Fahrenheit, just replace all instances of 'Celsius'.
*/

#include <SoftwareSerial.h>
#include "Kozir.h"
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t. Get it at https://www.pjrc.com/teensy/td_libs_DS1307RTC.html

SoftwareSerial nss(2,3); // Pin 2 = Arduino receiver pin (Rx), connect to sensor transmitter pin (Tx). Pin 3 = Arduino Tx, connect it to sensor Rx.
COZIR czr(nss);

// Time stamp
tmElements_t currentTime;

static bool waitingForSensor = false;
static byte type; // type of data: 0 = temperature, 1 = humidity, 2 = CO2

void setup(){
  Serial.begin(9600); 
  delay(5000); // The sensor needs a few seconds to calibrate.

  czr.getResponseCO2(); // Empty sensor serial buffer in case it still contains data.
}

void loop()
{
  RTC.read(currentTime);// Get time from real time clock
  int currentMinute = currentTime.Minute;
  static int lastPoll;// last poll moment
  
  // Request temperature, humidity and CO2 every minute:
  if ((currentMinute - lastPoll < 59) && (currentMinute > lastPoll) && (waitingForSensor == false)){

    // if (type == 0){
    //   czr.requestCelsius();
    // }
    // if (type == 1){
    //   czr.requestHumidity();      
    // }
    // if (type == 2) {
    //   czr.requestCO2();
    //   lastPoll = currentMinute;      
    //   if (currentMinute >= 59){ // Reset minute counter at end of the hour
    //     lastPoll = 0;
    //   }
    // }
    czr.requestHumidity();
    lastPoll = currentMinute;    
    waitingForSensor = true; // Don't allow data requests until sensor's buffer has been read.
  }
  else if (waitingForSensor) { // A request has been made, so now we can ask the sensor to return something
    float response;
    response = czr.getResponseHumidity();
    Serial.println("humidity:");
    Serial.println(response);

    // if (type == 2) { // Reversed order because of type++ after receiving useful data.
    //   response = (float) czr.getResponseCO2();
    //   if (response != (uint16_t) -1) {
    //     Serial.println("CO2:");
    //   }
    // }
    // if (type == 1){
    //   response = czr.getResponseHumidity();
    //   if (response != (uint16_t) -1) {
    //     Serial.println("humidity:");
    //   }
    // }
    // if (type == 0){
    //   response = czr.getResponseCelsius();
    //   if (response != (uint16_t) -1) {
    //     Serial.println("temperature:");
    //   }
    // }
    handleResponse(response);
  }
}
byte handleResponse(float response){
  if (response != (uint16_t) -1) {
    // type++;
    // if (type > 2){
    //   type = 0;
    // }
    waitingForSensor = false;
    //Serial.println(response);
  }
}