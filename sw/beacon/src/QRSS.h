

#ifndef QRSS_MODE
#define QRSS_MODE

#include "./lib/AD9850.h"

class QRSS {
  public:
    QRSS(AD9850 *oscillator);

  private:
    AD9850 *_oscillator;
};

#endif