

#include "AD9850.h"
#include <SPI.h>

SPIClass myInterface(SPI_N);

AD9850::AD9850() {}

void AD9850::begin(int select, int resetPin, int FQUDPin) {
    _select = select;
    _reset = resetPin;
    _fqud = FQUDPin;
    pinMode(_select, OUTPUT);
    pinMode(_reset, OUTPUT);
    pinMode(_fqud, OUTPUT);
    digitalWrite(_select, LOW);
    digitalWrite(_reset, LOW);
    digitalWrite(_fqud, LOW);

    myInterface.begin(); // set MOSI & MISO pin
                         // right.
    reset();
}

void AD9850::reset() {
    pulsePin(_reset);
    pulsePin(SPI_CLOCK);
    _config = 0;
    _freq = 0;
    _factor = 0;
    writeData();
}

void AD9850::powerDown() {
    _config |= AD985X_POWERDOWN; // keep phase and REFCLK as is.
    writeData();
}

void AD9850::powerUp() {
    _config &= ~AD985X_POWERDOWN; // TODO MAGIC NR.
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

    myInterface.beginTransaction(SPISettings(2000000, LSBFIRST, SPI_MODE0));
    digitalWrite(_select, LOW);
    myInterface.transfer(data & 0xFF);
    data >>= 8;
    myInterface.transfer(data & 0xFF);
    data >>= 8;
    myInterface.transfer(data & 0xFF);
    myInterface.transfer(data >> 8);
    myInterface.transfer(_config);
    digitalWrite(_select, HIGH);
    myInterface.endTransaction();

    // update frequency + phase + control bits.
    pulsePin(_fqud);
}

void AD9850::setFrequency(uint32_t freq) {
    // fOUT = (Δ Phase × CLKIN)/2^32
    // 64 bit math to keep precision to the max
    _freq = freq;
    _factor = (147573952590ULL * freq) >> 32; //  (1 << 64) / 125000000
    writeData();
}