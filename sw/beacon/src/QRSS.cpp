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
    delay(100000);
    _oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(100000);
    _oscillator->setFrequency(freq);
}

void QRSS::generateTestSequence(uint16_t hold, uint32_t frequency,
                                uint8_t deviation) {
    delay(hold);
    _oscillator->setFrequency(frequency);
    delay(hold);
    _oscillator->setFrequency(frequency + deviation);
    delay(hold);
    fskStop();
    delay(hold);
}

void QRSS::testCWMessage() {
    dot();
    dot();
    stop();
    dash();
    stop();
    dot();
    dot();
    dash();
    stop();
    dot();
    dot();
    stop();
    dash();
    stop();
}

void QRSS::carrierOn() {
    _oscillator->setFrequency(_baseFrequency);
}

void QRSS::carrierOff() {
    _oscillator->setFrequency(0);
}

void QRSS::fskMessage() {
    fskCWdot();
    delay(5000);
    fskCWdash();
    delay(5000);
}

void QRSS::dot() {
    carrierOn();
    delay(DOT_CW);
    carrierOff();
    delay(100);
}

void QRSS::dash() {
    carrierOn();
    delay(DOT_CW * WEIGHT);
    carrierOff();
    delay(100);
}

void QRSS::stop() {
    delay(DOT_CW * 3);
}

void QRSS::fskStop() {
    _oscillator->setFrequency(0);
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

uint8_t QRSS::charCode(char c) {
    switch (c) {
    case 'A':
        return 0b11111001; // A  .-
    case 'B':
        return 0b11101000; // B  -...
    case 'C':
        return 0b11101010; // C  -.-.
    case 'D':
        return 0b11110100; // D  -..
    case 'E':
        return 0b11111100; // E  .
    case 'F':
        return 0b11100010; // F  ..-.
    case 'G':
        return 0b11110110; // G  --.
    case 'H':
        return 0b11100000; // H  ....
    case 'I':
        return 0b11111000; // I  ..
    case 'J':
        return 0b11100111; // J  .---
    case 'K':
        return 0b11110101; // K  -.-
    case 'L':
        return 0b11100100; // L  .-..
    case 'M':
        return 0b11111011; // M  --
    case 'N':
        return 0b11111010; // N  -.
    case 'O':
        return 0b11110111; // O  ---
    case 'P':
        return 0b11100110; // P  .--.
    case 'Q':
        return 0b11101101; // Q  --.-
    case 'R':
        return 0b11110010; // R  .-.
    case 'S':
        return 0b11110000; // S  ...
    case 'T':
        return 0b11111101; // T  -
    case 'U':
        return 0b11110001; // U  ..-
    case 'V':
        return 0b11100001; // V  ...-
    case 'W':
        return 0b11110011; // W  .--
    case 'X':
        return 0b11101001; // X  -..-
    case 'Y':
        return 0b11101011; // Y  -.--
    case 'Z':
        return 0b11101100; // Z  --..
    case '0':
        return 0b11011111; // 0  -----
    case '1':
        return 0b11001111; // 1  .----
    case '2':
        return 0b11000111; // 2  ..---
    case '3':
        return 0b11000011; // 3  ...--
    case '4':
        return 0b11000001; // 4  ....-
    case '5':
        return 0b11000000; // 5  .....
    case '6':
        return 0b11010000; // 6  -....
    case '7':
        return 0b11011000; // 7  --...
    case '8':
        return 0b11011100; // 8  ---..
    case '9':
        return 0b11011110; // 9  ----.
    case ' ':
        return 0b00000000; // Space
    case '/':
        return 0b11010010; // /  -..-.
    default:
        return 0b00000000; // Space
    }
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
}

void QRSS::space() {
    stop();
}

void QRSS::txMessage(char *word) {

    uint16_t messageLength = strlen(word);

    // It will go through the message character by character and send it
    for (uint16_t myChar = 0; myChar < strlen(word); myChar++) {
        // printf("%c", word[myChar]);
        txLetter(charCode((uint8_t)word[myChar]));
        // printf("\n");
    }
}