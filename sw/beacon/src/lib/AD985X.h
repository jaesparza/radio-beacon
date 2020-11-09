#include "../../HardwareConfig.h"
#include "Arduino.h"

#define AD9850_MAX_FREQ  (20UL * 1000UL * 1000UL)
#define AD985X_POWERDOWN 0x04

class AD985X {
  public:
    AD985X();

    // for HW SPI only use lower 3 parameters.
    void begin(int select, int resetPin, int FQUDPin);
    void reset();
    void powerDown();
    void powerUp();

    // 0 .. 10.000.000(?)
    void setFrequency(uint32_t freq); // = 0  produces right error message
    uint32_t getFrequency() {
        return _freq;
    };

    // 0 .. 31  steps of 11.25 degrees
    void setPhase(uint8_t phase = 0);

    uint8_t getPhase() {
        return (_config >> 3);
    };

    void pulsePin(uint8_t pin);
    void writeData();

  private:
    uint8_t _select = 0;
    uint32_t _freq = 1;
    uint32_t _factor = 0;
    uint8_t _config = 0;
    uint8_t _reset = 0;
    uint8_t _fqud = 0;
};