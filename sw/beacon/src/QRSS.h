

#ifndef QRSS_MODE
#define QRSS_MODE

#include "./lib/AD9850.h"

#define DOT  0
#define DASH 10

class QRSS {
  public:
    QRSS(AD9850 *oscillator);

    void generateTestWSPR();
    void generateTestSequence(uint16_t delay, uint32_t frequency,
                              uint8_t deviation);
    void setBaseFrequency(uint32_t _baseFrequency);
    void fskMessage();
    void fskCWdot();
    void fskCWdash();

  private:
    AD9850 *_oscillator;
    uint32_t _baseFrequency;
};

#endif
