

#ifndef TIME
#define TIME

#include "../HardwareConfig.h"
#include "./lib/TinyGPSPlus.h"

#include <RTClock.h>

class TimeSync {
  public:
    uint8_t syncRTC();
    void monitorRTC();
    void getGPStime();

    void scheduleNextWSPRTX(voidFuncPtr triggeredFunction, uint16_t minutes);

    void NMEAstringGrabber();
    void registerAlarmFuntion(voidFuncPtr triggeredFunction,
                              uint8_t secondsAhead);
    void printTime(tm_t mtt);
    TimeSync(RTClock *rt, TinyGPSPlus *gps);

  private:
    RTClock *_rt;
    TinyGPSPlus *_gps;
    tm_t _mtt;
};

#endif