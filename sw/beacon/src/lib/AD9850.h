
/*
 * Filename: AD9850.h
 *
 * Based on the library from R. Tilard
 *     https://github.com/RobTillaart/AD985X
 *
 * - Congigurable to use a diferent SPI interface number than the primary one.
 * - Simplifed to be used only with AD9850 for SPI interface.
 * - Removed begin method and add constructor and initalization method.
 * - Added calibration method
 */

#ifndef AD9850_DRIVER
#define AD9850_DRIVER

#include "Arduino.h"

#define AD9850_MAX_FREQ  (20UL * 1000UL * 1000UL)
#define AD985X_POWERDOWN 0x04

class AD9850 {
  public:
    AD9850(int spiInt, int spiClock, int select, int resetPin, int FQUDPin);
    void init();
    void reset();
    void powerDown();
    void powerUp();

    void setCalibration(uint16_t calibration);

    void setFrequency(uint32_t freq);
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
    uint8_t _spiClock = 0;
    uint16_t _calibration = 0;
};

#endif