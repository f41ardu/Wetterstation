#include "GP2Y1010AU0F.h"

GP2Y1010AU0F::GP2Y1010AU0F():
  ledPin(0), measurePin(0)
{
  // Default Constructor
}

GP2Y1010AU0F::GP2Y1010AU0F(int _ledPin, int _measurePin, int _avrgCycles):
  ledPin(_ledPin), measurePin(_measurePin), avrgCycles(_avrgCycles)
  // SensorPin initialisieren
{
  pinMode(ledPin, OUTPUT);
}

// Destructor
GP2Y1010AU0F::~GP2Y1010AU0F()
// Destructore
{

}
double GP2Y1010AU0F::raw()
{
  unsigned int voltSum = 0;
  // Sensor auslesen
  for(int i=1; i<=avrgCycles; ++i)
  {
    digitalWrite(ledPin, LOW); // power on the LED
    delayMicroseconds(samplingTime);
    voltSum += analogRead(measurePin); // read the dust value
    delayMicroseconds(deltaTime);
    digitalWrite(ledPin, HIGH); // turn the LED off
    delayMicroseconds(sleepTime);
  };
  sigVoltage = double(voltSum) / avrgCycles * (5.0 / 1024.0);
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  //calDensity = 0.172 * sigVoltage - 0.0999;
  calDensity = 0.17 * sigVoltage - 0.1;
  return sigVoltage;
  
}

double GP2Y1010AU0F::voltage()
{  
  return sigVoltage;
}

double GP2Y1010AU0F::density()
{
  raw();
  if (calDensity <= 0.0) {
    Serial.println(calDensity);
    return NAN;
  } else {
    return calDensity;
  }
}



