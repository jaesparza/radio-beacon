

#ifndef QRSS_MODE
#define QRSS_MODE

#include "./lib/AD9850.h"

#include <string.h>

#define DASH_WEIGHT 3
#define DOT_CW      100

#define QRSS_DOT    2000
#define QRSS_WEIGHT 3
#define QRSS_DELAY  2000

#define FSK_HIGH 10

class QRSS {
  public:
    QRSS(AD9850 *oscillator);
    QRSS();
    void setBaseFrequency(uint32_t _baseFrequency);
    void fskMessage();
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

class CW_SENDER : public QRSS {
  public:
    CW_SENDER(AD9850 *oscillator);
    void dot();
    void dash();
    void stop();
    void space();
};

class FSK_SENDER : public QRSS {
  public:
    FSK_SENDER(AD9850 *oscillator);
    void dot();
    void dash();
    void stop();
    void space();
};

#endif
