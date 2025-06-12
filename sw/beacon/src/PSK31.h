#ifndef PSK31_MODE
#define PSK31_MODE

#include "../HardwareConfig.h"
#include "./lib/AD9850.h"

#define PSK31_BIT_DELAY 32 // ms per bit at 31.25 baud

class PSK31 {
  public:
    PSK31(AD9850 *oscillator);
    void setBaseFrequency(uint32_t baseFrequency);
    void sendPSK31Message(const char *message);

  private:
    AD9850 *_oscillator;
    uint32_t _baseFrequency;
    void setPhaseBit(bool bit);
    void sendVaricode(uint16_t code, uint8_t bits);
};

#endif // PSK31_MODE
