/*
 * Ombrometer Code 
 */
// SGHWeather v11.3
// Interrupt counter Ombrometer
void countOmbro() {
  ombro_ct ++;
}

// average rainfall per minute
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

