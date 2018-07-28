//  -------------------------------------------
//  |                                         |
//  |  Thingspeak-Website:                    |
//  |  https://thingspeak.com/channels/299338 |
//  |                                         |
//  -------------------------------------------
// SGHWeather v11.2

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Nano      9         8           10

// Bibliotheken
#include "src/AltSoftSerial/AltSoftSerial.h"
#include "src/EnableInterrupt/EnableInterrupt.h"
#include "src/GP2Y1010AU0F/GP2Y1010AU0F.h"
#include "src/UVlib/UVlib.h"
#include "src/DHT/DHT.h"

#include "BMP180wrapper.h"

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

AltSoftSerial ESP05(8, 9); //RX und TX Pins

// Bosch Drucksensor 
const float altitude = 485.590; // Altitude of SGH in Holzerlingen in meters
BMP180wrapper pressure(altitude);

// Staubsensor 
GP2Y1010AU0F DustSensor(pin_dustLedPower, pin_dust);

// UV Sensor 
UVlib UV(pin_UV_out, pin_UV_ref3V3);

// Luftfeuchtigkeitssensor 
DHT dht(pin_DHT, DHT22);

// Anemometer
volatile int wind_ct = 0;
unsigned long wind_lastTime = 0;

//Ombrometer
volatile int ombro_ct = 0;
unsigned long ombro_lastTime = 0;

// Thinkspeak Verbindung 
String domain = "184.106.153.149";
String ssid = "PhysComp";
String password = "XXXXXXXXXXX";

String thingspeakKey = "";

boolean errorCondition = false;

// Hier geht es los
void setup() {
  Serial.begin(9600);
  ESP05.begin(9600);
  pressure.begin();
  dht.begin(); 
  enableInterrupt(pin_anemometer, countWind, RISING);
  enableInterrupt(pin_ombrometer, countOmbro, RISING);
  delay(5000);
}

void loop() {
  
  float fields[8] = {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN};
  errorCondition = false;
  
  if( pressure.measure())
  {
    fields[0] = pressure.getP0();
    fields[1] = pressure.getP();
    fields[2] = (pressure.getT() + dht.readTemperature()) / 2.0;
   }else
  {
    fields[0] = fields[1]  = fields[2] = NAN;
  }
  fields[3] = DustSensor.density();
  fields[4] = UV.read();
  fields[5] = dht.readHumidity();
  
// Warten ohne Delay
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    if (MeasureState == LOW) {
      MeasureState = HIGH;
      fields[6] = anemometer();
      fields[7] = ombrometer();
      uploadThingspeak(thingspeakKey, fields);
    }
    else {
     MeasureState = LOW;
     fields[6] = anemometer();
     fields[7] = ombrometer();
     uploadThingspeak(thingspeakKey, fields);
    }
  }
}

void countOmbro() {
  ombro_ct ++;
}

void countWind() {
  wind_ct ++;
}

float anemometer() {

  const float gaugeFact = 0.32;
  unsigned long curTime = millis();
  unsigned int dt = curTime - wind_lastTime;
  wind_lastTime = curTime;
  float speed = float(wind_ct) / (dt / 1000.0) * gaugeFact;
  Serial.println("dt_wind: " + String(dt));
  Serial.println("wind_ct: " + String(wind_ct));
  speed = speed / 3.6;
  wind_ct = 0;
  return speed;
}

float ombrometer() {

  const float volume = 1;
  const float surface = 1;
  unsigned long curTime = millis();
  unsigned int dt = curTime - ombro_lastTime;
  ombro_lastTime = curTime;
  float water = float (ombro_ct * volume * 4) / surface;
  Serial.println("dt_ombro: " + String(dt));
  Serial.println("ombro_ct: " + String(ombro_ct));
  ombro_ct = 0;
  return water;
}

