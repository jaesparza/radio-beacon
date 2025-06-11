/*
 * Filename: QRSS.cpp
 * Description: Implementation of methods for the classess QRSS, FSK_SENDER and
 * CW_SENDER.
 *
 *  Author: ja - jaesparza@gmail.com
 */

#include "QRSS.h"
#include <ctype.h>

QRSS::QRSS(AD9850 *oscillator) {
    _oscillator = oscillator;
}

QRSS::QRSS() {}

FSK_SENDER::FSK_SENDER(AD9850 *oscillator) {
    _oscillator = oscillator;
}

void QRSS::setBaseFrequency(uint32_t baseFrequency) {
    _baseFrequency = baseFrequency;
}

CW_SENDER::CW_SENDER(AD9850 *oscillator) {
    _oscillator = oscillator;
}

void CW_SENDER::dot() {
    _oscillator->setFrequency(_baseFrequency);
    delay(DOT_CW);
    _oscillator->setFrequency(0);
    delay(CW_DELAY); // pause after symbol set to one dot duration
}

void CW_SENDER::dash() {
    _oscillator->setFrequency(_baseFrequency);
    delay(DOT_CW * DASH_WEIGHT);
    _oscillator->setFrequency(0);
    delay(CW_DELAY); // pause after symbol set to one dot duration
}

void CW_SENDER::stop() {
    _oscillator->setFrequency(0);
}

void CW_SENDER::space() {
    delay(DOT_CW * 3);
}

// These are the methods to do FSK
void FSK_SENDER::dot() {
    _oscillator->setFrequency(_baseFrequency + FSK_HIGH);
    delay(QRSS_DOT);
    _oscillator->setFrequency(_baseFrequency);
    delay(QRSS_DELAY);
}

void FSK_SENDER::dash() {
    _oscillator->setFrequency(_baseFrequency + FSK_HIGH);
    delay(QRSS_DOT * QRSS_WEIGHT);
    _oscillator->setFrequency(_baseFrequency);
    delay(QRSS_DELAY);
}

void FSK_SENDER::stop() {
    _oscillator->setFrequency(_baseFrequency);
}

void FSK_SENDER::space() {
    delay(QRSS_DOT * 3);
}

void QRSS::txLetter(uint8_t character) {

    uint8_t symbol = 7;

    if (character == 0) {
        space();
        return;
    }

    // Finds the first symbol
    while (character & 1 << symbol) {
        symbol--;
    }

    while (symbol != 0) {
        symbol--;
        if (character & 1 << symbol) {
            dash();
        } else {
            dot();
        }
    }
    stop();
    space();
}

void QRSS::txMessage(char *word) {

    uint16_t messageLength = strlen(word);

    // It will go through the message character by character and send it
    for (uint16_t myChar = 0; myChar < messageLength; myChar++) {
        txLetter(charCode((uint8_t)word[myChar]));
    }
}

uint8_t QRSS::charCode(char c) {
    struct MorseCodeMap {
        char    symbol;
        uint8_t code;
    };

    static const MorseCodeMap lookup[] = {
        {'A', 0b11111001}, // .-
        {'B', 0b11101000}, // -...
        {'C', 0b11101010}, // -.-.
        {'D', 0b11110100}, // -..
        {'E', 0b11111100}, // .
        {'F', 0b11100010}, // ..-.
        {'G', 0b11110110}, // --.
        {'H', 0b11100000}, // ....
        {'I', 0b11111000}, // ..
        {'J', 0b11100111}, // .---
        {'K', 0b11110101}, // -.-
        {'L', 0b11100100}, // .-..
        {'M', 0b11111011}, // --
        {'N', 0b11111010}, // -.
        {'O', 0b11110111}, // ---
        {'P', 0b11100110}, // .--.
        {'Q', 0b11101101}, // --.-
        {'R', 0b11110010}, // .-.
        {'S', 0b11110000}, // ...
        {'T', 0b11111101}, // -
        {'U', 0b11110001}, // ..-
        {'V', 0b11100001}, // ...-
        {'W', 0b11110011}, // .--
        {'X', 0b11101001}, // -..-
        {'Y', 0b11101011}, // -.--
        {'Z', 0b11101100}, // --..
        {'0', 0b11011111}, // -----
        {'1', 0b11001111}, // .----
        {'2', 0b11000111}, // ..---
        {'3', 0b11000011}, // ...--
        {'4', 0b11000001}, // ....-
        {'5', 0b11000000}, // .....
        {'6', 0b11010000}, // -....
        {'7', 0b11011000}, // --...
        {'8', 0b11011100}, // ---..
        {'9', 0b11011110}, // ----.
        {' ', 0b00000000}, // space
        {'/', 0b11010010}  // -..-.
    };

    c = toupper(static_cast<unsigned char>(c));

    for (const auto &entry : lookup) {
        if (entry.symbol == c) {
            return entry.code;
        }
    }

    return 0b00000000; // Default: space
}
