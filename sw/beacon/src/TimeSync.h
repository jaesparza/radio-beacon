

#ifndef TIME
#define TIME

#include "./lib/TinyGPSPlus.h"
#include <RTClock.h>

class TimeSync {

  private:
  public:
    // Reference to the RTC
    RTClock *rt;
    tm_t mtt;
    // Reference to the GPS
};

#endif