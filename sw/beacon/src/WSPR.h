
/*
 * Filename: WSPR.h
 * Description: WSPR message sender.
 * Author: ja - jaesparza@gmail.com
 */

#ifndef WSPR_MODE
#define WSPR_MODE

#include "../HardwareConfig.h"
#include "./lib/AD9850.h"

#include <string.h>

// Keep in mind that WSPR_TONE_SEPARATION is treated as a double by the
// compiler. Cast it as needed before providing this value to the oscillator
#define WSPR_TONE_SEPARATION 1.468

#define WSPR_MESSAGE_LENGTH    162
#define WSPR_INTERSYMBOL_DELAY 683

class WSPR {
  public:
    WSPR(AD9850 *oscillator);
    void setBaseFrequency(uint32_t _baseFrequency);
    void generateTestTones();
    void sendWSPRmessage();

  private:
    uint32_t _baseFrequency;
    AD9850 *_oscillator;
    void generateTone(uint8_t t);
};

#endif