
/*
 * Filename: QRSS.h
 * Description:
 *  This file contains the following classes:
 *   - QRSS: interfaces the oscillator, sets the base frequency for the
 *     transmission, and goe through a string message and sends it.
 *   - CW_SENDER: implementation of the CW modulation with carrier ON/OFF keying
 *   - FSK_SENDER: implementation of the FSK CW modulation.
 *
 * Author: ja - jaesparza@gmail.com
 */

#ifndef QRSS_MODE
#define QRSS_MODE

#include "./lib/AD9850.h"

#include <string.h>

#define DASH_WEIGHT 3
#define DOT_CW      100
#define CW_DELAY    DOT_CW

#define QRSS_DOT    2000
#define QRSS_WEIGHT 3
#define QRSS_DELAY  2000

#define FSK_HIGH 10

class QRSS {
  public:
    QRSS(AD9850 *oscillator);
    QRSS();
    void setBaseFrequency(uint32_t _baseFrequency);
    void txMessage(char *word);

  protected:
    AD9850 *_oscillator;
    uint32_t _baseFrequency;

  private:
    uint8_t charCode(char c);
    void txLetter(uint8_t character);

    virtual void dot() = 0;
    virtual void dash() = 0;
    virtual void stop() = 0;
    virtual void space() = 0;
};

/*
 * Class implementing traditional ON/OFF keying for CW
 *
 * DASH duration = DOT * DASH_WEIGHT
 *
 */
class CW_SENDER : public QRSS {
  public:
    CW_SENDER(AD9850 *oscillator);
    void dot();
    void dash();
    void stop();
    void space();
};

/*
 * Class implementing the Frequency Shift Keying (FSK) for CW
 *
 * DASH duration = DOT * QRSS_WEIGHT
 *
 *                                  DOT      DASH       DOT
 * Frequency Base + FSK_HIGH         ----    --------    ----
 *  .                                    |   |       |   |   |
 *  .                                    |   |       |   |   |
 * Frequency Base                        ----         ----    ----
 */

class FSK_SENDER : public QRSS {
  public:
    FSK_SENDER(AD9850 *oscillator);
    void dot();
    void dash();
    void stop();
    void space();
};

#endif
