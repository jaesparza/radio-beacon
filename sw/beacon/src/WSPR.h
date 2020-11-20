
/*
 * Filename: WSPR.h
 * Description: WSPR message sender.
 * Author: ja - jaesparza@gmail.com
 */

#ifndef WSPR_MODE
#define WSPR_MODE

#include "./lib/AD9850.h"
#include <string.h>

#define TONE_SEPARATION 1.468

class WSPR {
  public:
    WSPR(AD9850 *oscillator);
    void setBaseFrequency(uint32_t _baseFrequency);
    void generateTestWSPRsequence();

  private:
    uint32_t _baseFrequency;
    AD9850 *_oscillator;

    void sendTone(uint8_t tone);
};

#endif
