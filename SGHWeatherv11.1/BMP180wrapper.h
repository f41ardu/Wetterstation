#ifndef BMP180WRAPPER_h
#define BMP180WRAPPER_h

#include "SFE_BMP180.h"


class BMP180wrapper{
 public:
  BMP180wrapper( float altitude);
  ~BMP180wrapper();
  void begin();
  bool measure();
  double getT();
  double getP();
  double getP0();
 private:
  SFE_BMP180 bmp;
  float _altitude;
  double T;
  double P;
  double P0;
  bool status;
};

#endif
