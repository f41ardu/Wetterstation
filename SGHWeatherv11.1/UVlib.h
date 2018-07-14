#ifndef UVlib_h
#define UVlib_h
     
#include <Arduino.h> 

class UVlib
{
  public:                              // öffentlich
    UVlib();                      // der Default-Konstruktor
    UVlib(int, int);           // weiterer Konstruktor mit Parameter 
    unsigned int averageAnalogRead(int);


    float read();                // eine Funktion mit einem (Default-) Parameter LED Initialisieresn

  private:                             // privat
    int _UVOUT;
    int _REF_3V3;
    
};

#endif
