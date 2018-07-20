#include "Arduino.h"
#include "UVlib.h"

//Das ist die cpp-Datei

UVlib::UVlib():
_UVOUT(0),_REF_3V3(0)
{

}

UVlib::UVlib(int UVOUT, int REF_3V3):
_UVOUT(UVOUT), _REF_3V3(REF_3V3)
{
  // UVOUT A0
  // REF_3V3 A3
  pinMode(_UVOUT, INPUT);
  pinMode(_REF_3V3, INPUT);
}

float UVlib::read()
{

  int uvLevel = averageAnalogRead(_UVOUT);
  int refLevel = averageAnalogRead(_REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;

  float uvIntensity = (outputVoltage - 0.99) * (15.0 - 0.99) / (15.0 - 0.0) + 0.99; //Convert the voltage to a UV intensity level
  return uvIntensity;
} 

//Takes an average of readings on a given pin
//Returns the average
unsigned int UVlib::averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
    
  return(runningValue);  
}



