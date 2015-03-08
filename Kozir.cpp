// 
//       FILE: Cozir.cpp
//     AUTHOR: Rob Tillaart & Michael Hawthorne
//    VERSION: 0.1.01
//    PURPOSE: library for COZIR range of sensors for Arduino
//        URL: http://www.cozir.com/
//  DATASHEET: http://www.co2meters.com/Documentation/Datasheets/COZIR-Data-Sheet-RevC.pdf
// USER GUIDE: http://www.co2meters.com/Documentation/Manuals/COZIR-Software-User-Guide-AL12-RevA.pdf
// READ DATASHEET BEFORE USE OF THIS LIB !
//
// Released to the public domain
//

#include "Kozir.h"
#include "SoftwareSerial.h"

uint32_t time_request_sent;// Time stamp for last request to sensor in millis.
#define RESPONSE_NOT_READY ((uint16_t) -1)

////////////////////////////////////////////////////////////
//
// CONSTRUCTOR
//
COZIR::COZIR(SoftwareSerial& nss) : CZR_Serial(nss)
{
  CZR_Serial.begin(9600);
}


////////////////////////////////////////////////////////////
//
// POLLING MODE
//
// you need to set the polling mode explicitely before 
// using these functions.
//
// 
void COZIR::requestCelsius(){
  sendRequest("T");
}
float COZIR::getResponseCelsius(){
  uint16_t rv = getResponse();

  if (rv == RESPONSE_NOT_READY){
    return RESPONSE_NOT_READY;
  }
  else{
    if (rv < 1000){
      return 0.1 * rv;
    }
    else{
      return -0.1 * (rv-1000);
    }    
  }
}
void COZIR::requestHumidity(){  
  sendRequest("H");
}
float COZIR::getResponseHumidity(){
  uint16_t rv = getResponse();

  if (rv == RESPONSE_NOT_READY){
    return RESPONSE_NOT_READY;
  }
  else{
    return 0.1 * getResponse();
  }
}
void COZIR::requestCO2(){
  sendRequest("Z");
}
float COZIR::getResponseCO2(){ // This can be returned as uint16_t as well.
  uint16_t rv = getResponse();

  if (rv == RESPONSE_NOT_READY){
    return RESPONSE_NOT_READY;
  }
  else{
    return getResponse();
  }
}

/////////////////////////////////////////////////////////
// PRIVATE
  
void COZIR::Command(char* s)
{
  CZR_Serial.print(s);
  CZR_Serial.print("\r\n");
}
void COZIR::sendRequest(char* s){
  Command(s);
  time_request_sent = millis();
}
uint16_t COZIR::getResponse(){
  if (millis() - time_request_sent < 200){
    return RESPONSE_NOT_READY;
  }
  buffer[0] = '\0';  // empty buffer
  int idx = 0;
  while(CZR_Serial.available()){
    buffer[idx++] = CZR_Serial.read();
  }
  buffer[idx] = '\0';
  uint16_t rv = 0;

  switch(buffer[1])
  {
    case 'T' :
            rv = atoi(&buffer[5]);
            if (buffer[4] == 1) rv += 1000;
            break;
    default :
            rv = atoi(&buffer[2]);
            break;
  }  
  return rv;
}