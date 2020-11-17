

#ifndef QRSS_MODE
#define QRSS_MODE

#include "./lib/AD9850.h"

#include <string.h>

#define DOT  0
#define DASH 10

#define WEIGHT 3

#define DOT_CW 100

class QRSS {
  public:
    QRSS(AD9850 *oscillator);

    void generateTestWSPR();
    void generateTestSequence(uint16_t delay, uint32_t frequency,
                              uint8_t deviation);
    void setBaseFrequency(uint32_t _baseFrequency);

    void fskMessage();
    void testCWMessage();

    void txMessage(char *word);

  private:
    AD9850 *_oscillator;
    uint32_t _baseFrequency;

    uint8_t charCode(char c);
    void txLetter(uint8_t character);
    void fskCWdot();
    void fskCWdash();
    void fskStop();
    void carrierOff();
    void carrierOn();
    void dot();
    void dash();
    void stop();
    void space();
};

#endif
