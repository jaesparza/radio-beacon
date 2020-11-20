#include "WSPR.h"

WSPR::WSPR(AD9850 *oscillator) {
    _oscillator = oscillator;
}

void WSPR::generateTestWSPRsequence() {
    uint32_t freq = 0;
    freq = 10140250 + 1.468;
    delay(10000);
    _oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(100000);
    _oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(100000);
    _oscillator->setFrequency(freq);
}

void WSPR::setBaseFrequency(uint32_t baseFrequency) {
    _baseFrequency = baseFrequency;
}

void WSPR::sendTone(uint8_t tone) {
    switch (tone) {
    case 1:
        // TX in base frequency
        break;
    case 2:
        _baseFrequency = _baseFrequency + (TONE_SEPARATION * 2);
        break;
    case 3:
        _baseFrequency = _baseFrequency + (TONE_SEPARATION * 3);
        break;
    default:
        break;
    }
    _oscillator->setFrequency(_baseFrequency);
}