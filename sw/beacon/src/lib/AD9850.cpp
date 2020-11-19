/*
 * Filename: AD9850.cpp
 *
 * Based on the library from R. Tilard
 *     https://github.com/RobTillaart/AD985X
 *
 * Modifications described in AD9850.h
 */

#include "AD9850.h"
#include <SPI.h>

SPIClass *myInterface;

AD9850::AD9850(int spiInt, int spiClock, int select, int resetPin,
               int FQUDPin) {
    _select = select;
    _reset = resetPin;
    _fqud = FQUDPin;
    _spiClock = spiClock;
    myInterface = new SPIClass(spiInt);
}

void AD9850::setCalibration(uint16_t calibration) {
    _calibration = calibration;
}

void AD9850::init() {

    pinMode(_select, OUTPUT);
    pinMode(_reset, OUTPUT);
    pinMode(_fqud, OUTPUT);
    digitalWrite(_select, LOW);
    digitalWrite(_reset, LOW);
    digitalWrite(_fqud, LOW);

    myInterface->begin();
    reset();
}

void AD9850::reset() {
    pulsePin(_reset);
    pulsePin(_spiClock);
    _config = 0;
    _freq = 0;
    _factor = 0;
    writeData();
}

void AD9850::powerDown() {
    _config |= AD985X_POWERDOWN;
    writeData();
}

void AD9850::powerUp() {
    _config &= ~AD985X_POWERDOWN;
    writeData();
}

void AD9850::setPhase(uint8_t phase) {
    if (phase > 31)
        return;
    _config &= 0x07;
    _config |= (phase << 3);
    writeData();
}

void AD9850::pulsePin(uint8_t pin) {
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
}

void AD9850::writeData() {

    uint32_t data = _factor;

    myInterface->beginTransaction(SPISettings(2000000, LSBFIRST, SPI_MODE0));
    digitalWrite(_select, LOW);
    myInterface->transfer(data & 0xFF);
    data >>= 8;
    myInterface->transfer(data & 0xFF);
    data >>= 8;
    myInterface->transfer(data & 0xFF);
    myInterface->transfer(data >> 8);
    myInterface->transfer(_config);
    digitalWrite(_select, HIGH);
    myInterface->endTransaction();

    // update frequency + phase + control bits.
    pulsePin(_fqud);
}

void AD9850::setFrequency(uint32_t freq) {
    // fOUT = (Δ Phase × CLKIN)/2^32
    // 64 bit math to keep precision to the max
    _freq = freq;
    _factor = (147573952590ULL * (freq + (uint16_t)_calibration)) >>
              32; //  (1 << 64) / 125000000
    writeData();
}