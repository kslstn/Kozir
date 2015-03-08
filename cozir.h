// 
//       FILE: Cozir.h
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

#ifndef Cozir_h
#define Cozir_h

#include "SoftwareSerial.h"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define COZIR_LIB_VERSION 0.1.01

class COZIR
{
  public:
	COZIR(SoftwareSerial&);// : CZR_Serial(nss)
 
	void requestCelsius();  
	float getResponseCelsius();  
	void requestHumidity();  
	float getResponseHumidity();  
	void requestCO2();  
	float getResponseCO2();  
  
  private:
	void sendRequest(char* );	
	uint16_t getResponse();  
    SoftwareSerial& CZR_Serial;
    char buffer[20];
	void Command(char* );
};

#endif
