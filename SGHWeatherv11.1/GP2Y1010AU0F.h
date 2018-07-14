// http://de.wikibooks.org/wiki/C%2B%2B-Programmierung:_Klassen

#ifndef GP2Y1010AU0F_H
#define GP2Y1010AU0F_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"  // for digitalRead, digitalWrite, etc
#else
#include "WProgram.h"
#endif
//#include <Arduino.h> //It is very important to remember this! note that if you are using Arduino 1.0 IDE, change "WProgram.h" to "Arduino.h"


class GP2Y1010AU0F
{
  public:                             // öffentlich
    GP2Y1010AU0F();                     // der Default-Konstruktor
    GP2Y1010AU0F(int ledPin, int measurePin, int avrgCycles=1);             // weiterer Konstruktor mit Parameter
    //   PinClass(const LED& a);      // Copy-Konstruktor wird nicht benötigt
    ~GP2Y1010AU0F();                    // Class Destruktor

    double density();              // Staubdichte ausgeben µg/m^3
    double raw();                      // raw daten ausgeben
    double voltage();                  // spannung berechnen

  private:                            // privat
    const unsigned int samplingTime = 280;
    const unsigned int deltaTime = 40;
    const unsigned int sleepTime = 9680;

    const unsigned int ledPin, measurePin, avrgCycles;

    // double voMeasured = 0;
    double sigVoltage = 0;
    double calDensity = 0;
    //double _calcVoltage = 0; 
    //double _calDensity = 0; 

};

#endif // GP2Y1010AU0F_H

