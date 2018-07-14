/*

Hier wird die Reihenfolge der Werte die der Arduino
mit voller Bestückung hintereinander ausgibt.
Die Werte werden voneinander mit einem Kommas getrennnt:

  
1.    Der Luftdruck             (in hPa)        BMP180            [Variable: Druck.p0]              -> A4 (SDA), A5 (SCL), GND(GND), VCC(3,3V)
2.    Den absuluten Luftdruck   (in hPa)        BMP180            [Variable: Druck.P]               -> A4 (SDA), A5 (SCL), GND(GND), VCC(3,3V)
3.    Die Temperatur            (in °C)         BMP180 + DHT22    [Variable: temp]                  -> A4 (SDA), A5 (SCL), GND(GND), VCC(3,3V) + OUT(D3), +(5V), -(GND)
4.    Der Feinstaub             (in mg/m3)      GP2Y1010AU0F      [Variable: dust]                  -> mesurePin(grün) A6, ledpowerPin(orange) D12
5.    UV-Sensor                 (UV-Index)      ML8511            [Variable: uv_rad]                -> EN(3.3V), OUT(A2), GND(GND), 3.3V(3.3V)
6.    Die Luftfeuchtigkeit      (in %)          DHT22             [Variable: h]                     -> OUT(D3), +(5V), -(GND)
7.    Die Windgeschwindigkeit   (in m/s)        Anemometer        [Variable: windSpeed]             -> pin 6
8.    Die Niederschlagsmenge    (mm/min)        Eigenproduktion   [Variable: rainAmmount]           -> pin 5


[4.1  Vom Feinstaubsensor       (in Volt)       GP2Y1010AU0F      Variable: DustSensor.voltage()    -> mesurePin(grün) A6, ledpowerPin(orange) D12]
*/
