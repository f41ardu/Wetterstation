//  -------------------------------------------
//  |                                         |
//  |  Thingspeak-Website:                    |
//  |  https://thingspeak.com/channels/299338 |
//  |                                         |
//  -------------------------------------------

// #include <SoftwareSerial.h>
#include "AltSoftSerial.h"

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Nano      9         8           10

// Bibliotheken
#include "EnableInterrupt.h"
#include "BMP180wrapper.h"
#include "GP2Y1010AU0F.h"
#include "UVlib.h"
#include "DHT.h"

// Pin-Definitionen
const int pin_UV_ref3V3    = 1;
const int pin_UV_out       = 2;
const int pin_DHT          = 3;
const int pin_dust         = A6;    //Connect dust sensor to Arduino A6 pin
const int pin_dustLedPower = 12;    //Connect D2 led driver pins of dust sensor to Arduino D2
const int pin_anemometer   = 6;
const int pin_ombrometer   = 5;
const int pin_serial_RX    = 8;
const int pin_serial_TX    = 9;

// Blink ohne Delay 
int MeasureState = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 15000;

// WLAN Verbindung 
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Nano        9         8         10

// SoftwareSerial ESP05(8, 9); //RX und TX Pins
AltSoftSerial ESP05(8, 9); //RX und TX Pins

// Bosch Drucksensor 
const float altitude = 485.590; // Altitude of SGH in Holzerlingen in meters
BMP180wrapper pressure(altitude);

// Staubsensor 
GP2Y1010AU0F DustSensor(pin_dustLedPower, pin_dust);

// UV Sensor 
UVlib UV( pin_UV_out, pin_UV_ref3V3);

// Luftfeuchtigkeitssensor 
DHT dht(pin_DHT, DHT22);

// Anemometer
volatile int wind_ct = 0;
unsigned long wind_lastTime = 0;

// Thinkspeak Verbindung 
String domain = "184.106.153.149";
String ssid = "PhysComp";
String password = "XXXXXXXXXXX";

String thingspeakKey = "YYYYYYYYYYYY";

boolean errorCondition = false;

// Hier geht es los
void setup() {
  Serial.begin(9600);
  ESP05.begin(9600);
  pressure.begin();
  dht.begin(); 
  enableInterrupt(pin_anemometer, countWind, RISING);
  delay(5000);
}

void loop() {
  
  float fields[] = {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN};
  errorCondition = false;
  
  if( pressure.measure())
  {
    fields[0] = pressure.getP0();
    fields[1] = pressure.getP();
    fields[2] = (pressure.getT() + dht.readTemperature())/2.0;
   }else
  {
    fields[0] = fields[1]  = fields[2] = NAN;
  }
  fields[3] = DustSensor.density();
  fields[4] = UV.read();
  fields[5] = dht.readHumidity();
//  fields[6] = anemometer();
  
// Warten ohne Delay 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    if (MeasureState == LOW) {
      MeasureState = HIGH;
      fields[6] = anemometer();
      uploadThingspeak(thingspeakKey, fields);
    }
    else {
     MeasureState = LOW;
     fields[6] = anemometer();
     uploadThingspeak(thingspeakKey, fields);
    }
  }
 
}

void countWind() {
  wind_ct ++;
}

float anemometer() {

  const float gaugeFact = 2.4;
  unsigned long curTime = millis();
  unsigned int dt = curTime - wind_lastTime;
  wind_lastTime = curTime;
  float speed = float(wind_ct) / (dt / 1000.0) * gaugeFact;
  Serial.println("dt: " + String(dt));
  Serial.println("wind_ct: " + String(wind_ct));
  speed = speed / 3.6;
  wind_ct = 0;
  if(speed == 0.0) speed = NAN;
  return speed;
}

