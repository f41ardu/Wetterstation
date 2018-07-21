/*
 * Helper code 
 */
// SGHWeather v11.3

#include "BMP180wrapper.h"


BMP180wrapper::BMP180wrapper(float altitude) : bmp()
{
  _altitude = altitude;
  //bmp = new SFE_BMP180;
  //status = bmp.begin();
};

BMP180wrapper::~BMP180wrapper()
{
  //delete bmp;
}

void BMP180wrapper::begin()
{
  status = bmp.begin();
}

bool BMP180wrapper::measure()
{
  T = P = P0 = NAN;
  unsigned short tm;
  status = true;  
  tm = bmp.startTemperature();
  if( tm == 0)
  {
    status = false;
    return status;
  };

  // Wait for the measurement to complete:
  delay(tm);

  // Retrieve the completed temperature measurement:
  // Note that the measurement is stored in the variable T.
  // Function returns 1 if successful, 0 if failure.

  status = bmp.getTemperature(T);
  if (!status){ return status;};

  // Start a pressure measurement:
  // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  tm = bmp.startPressure(3);
  if( tm == 0)
  {
    status = false;
    return status;
  };

  // Wait for the measurement to complete:
  delay(tm);

  // Retrieve the completed pressure measurement:
  // Note that the measurement is stored in the variable P.
  // Note also that the function requires the previous temperature measurement (T).
  // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
  // Function returns 1 if successful, 0 if failure.

  status = bmp.getPressure(P, T);
  if (!status){ return status;};
        

  // The pressure sensor returns abolute pressure, which varies with altitude.
  // To remove the effects of altitude, use the sealevel function and your current altitude.
  // This number is commonly used in weather reports.
  // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
  // Result: p0 = sea-level compensated pressure in mb
          
  P0 = bmp.sealevel(P, _altitude);
        
  return status;
};

double BMP180wrapper::getT()
{
  return T;
};

double BMP180wrapper::getP()
{
  return P;
};

double BMP180wrapper::getP0()
{
  return P0;
};


