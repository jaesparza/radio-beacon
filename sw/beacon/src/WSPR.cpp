/*
 * Filename: WSPR.cpp
 * Description: Implementation of methods for the WSPR sender.
 *  Author: ja - jaesparza@gmail.com
 */

#include "WSPR.h"

WSPR::WSPR(AD9850 *oscillator) {
    _oscillator = oscillator;
}

/*
 * This function will generate a sequence of test tones that can be used to
 * verify the correct operation of the oscillator at WSPR frequencies before
 * sending a complete message.
 */
void WSPR::generateTestTones() {
    for (uint8_t i = 0; i < 4; i++) {
        generateTone(i);
        delay(5000);
    }
}

void WSPR::setBaseFrequency(uint32_t baseFrequency) {
    _baseFrequency = baseFrequency;
}

/*
 * Example message from the WSPR user manual
 *
 * The standard format according to the protocol spec. is: "callsign + 4-digit
 * locator + dBm"
 * Channel symbols for the message "K1ABC FN42 37"
 */
uint8_t EXAMPLE_WSPR_DATA[] = {
    3, 3, 0, 0, 2, 0, 0, 0, 1, 0, 2, 0, 1, 3, 1, 2, 2, 2, 1, 0, 0, 3, 2, 3,
    1, 3, 3, 2, 2, 0, 2, 0, 0, 0, 3, 2, 0, 1, 2, 3, 2, 2, 0, 0, 2, 2, 3, 2,
    1, 1, 0, 2, 3, 3, 2, 1, 0, 2, 2, 1, 3, 2, 1, 2, 2, 2, 0, 3, 3, 0, 3, 0,
    3, 0, 1, 2, 1, 0, 2, 1, 2, 0, 3, 2, 1, 3, 2, 0, 0, 3, 3, 2, 3, 0, 3, 2,
    2, 0, 3, 0, 2, 0, 2, 0, 1, 0, 2, 3, 0, 2, 1, 1, 1, 2, 3, 3, 0, 2, 3, 1,
    2, 1, 2, 2, 2, 1, 3, 3, 2, 0, 0, 0, 0, 1, 0, 3, 2, 0, 1, 3, 2, 2, 2, 2,
    2, 0, 2, 3, 3, 2, 3, 2, 3, 3, 2, 0, 0, 3, 1, 2, 2, 2,
};

/*
 * My WSPR frame "OU2ECV JO65 23"
 * To generate your own under windows, execute from cmd:
 * c:\> WSPRcode "AB2CDE XX99 00"
 * where AB2CDE is your callsing, XX99 your 4 digit locator and 00 your tx power
 * in dbm.
 *
 * To convert watts into dbm in a 50 ohms system, you can use an online
 * calculator, a conversion tables, etc...
 */
uint8_t MY_WSPR_DATA[] = {
    3, 3, 2, 2, 2, 2, 2, 2, 3, 2, 2, 0, 1, 3, 1, 2, 0, 2, 1, 0, 2, 1, 0, 3,
    3, 3, 3, 0, 0, 0, 2, 2, 2, 0, 1, 2, 0, 1, 2, 1, 2, 0, 2, 2, 2, 0, 1, 0,
    3, 1, 0, 2, 1, 1, 2, 1, 2, 0, 0, 1, 3, 2, 3, 2, 0, 2, 0, 3, 1, 2, 1, 0,
    3, 0, 1, 0, 3, 0, 0, 3, 2, 2, 3, 2, 1, 1, 2, 2, 2, 3, 1, 2, 3, 0, 1, 0,
    0, 0, 1, 2, 2, 0, 0, 0, 3, 0, 2, 1, 0, 2, 3, 3, 3, 0, 1, 3, 0, 0, 3, 1,
    0, 1, 2, 0, 0, 3, 1, 3, 2, 0, 0, 0, 0, 3, 2, 1, 2, 0, 1, 3, 2, 0, 0, 0,
    2, 0, 2, 3, 3, 0, 3, 2, 1, 1, 0, 0, 0, 1, 1, 0, 2, 0,
};

void WSPR::generateTone(uint8_t t) {
    uint32_t tone = _baseFrequency + (t)*WSPR_TONE_SEPARATION;
    // The actual frequencies for each tone will be:
    //     (_baseFrequency + [0, 1, 3, 4])
    // this is due to the rounding of frequency after the tone calculation.
    _oscillator->setFrequency(tone);
}

void WSPR::sendWSPRmessage() {
    for (uint8_t i = 0; i < WSPR_MESSAGE_LENGTH; i++) {
        generateTone(MY_WSPR_DATA[i]);
        delay(WSPR_INTERSYMBOL_DELAY);
    }
    generateTone(0);
}