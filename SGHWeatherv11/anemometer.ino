/*
 * Anemometer Code 
 */
// SGHWeather v11.3
// Interrupt counter Anemometer
void countWind() {
  wind_ct ++;
}

// average Anemometer speed
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
