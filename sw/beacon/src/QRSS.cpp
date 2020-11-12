#include "QRSS.h"

QRSS::QRSS(AD9850 *oscillator) {
    _oscillator = oscillator;
}

void QRSS::generateTestWSPR() {
    uint32_t freq = 0;
    freq = 10140250 + 1.468;
    delay(10000);
    _oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(10000);
    _oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(10000);
    _oscillator->setFrequency(freq);
}

void QRSS::generateTestSequence(uint16_t hold, uint32_t frequency,
                                uint8_t deviation) {
    delay(hold);
    _oscillator->setFrequency(frequency);
    delay(hold);
    _oscillator->setFrequency(frequency + deviation);
}

void QRSS::fskMessage() {
    fskCWdot();
    delay(5000);
    fskCWdash();
    delay(5000);
}

void QRSS::fskCWdot() {
    _oscillator->setFrequency(_baseFrequency + DOT);
}

void QRSS::fskCWdash() {
    _oscillator->setFrequency(_baseFrequency + DASH);
}

void QRSS::setBaseFrequency(uint32_t baseFrequency) {
    _baseFrequency = baseFrequency;
}