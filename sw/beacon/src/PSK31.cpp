#include "PSK31.h"
#include <Arduino.h>

PSK31::PSK31(AD9850 *oscillator) {
    _oscillator = oscillator;
}

void PSK31::setBaseFrequency(uint32_t baseFrequency) {
    _baseFrequency = baseFrequency;
    _oscillator->setFrequency(_baseFrequency);
}

void PSK31::setPhaseBit(bool bit) {
    // 0 deg for logical 0, 180 deg for logical 1
    _oscillator->setPhase(bit ? 16 : 0);
}

void PSK31::sendVaricode(uint16_t code, uint8_t bits) {
    for (int8_t i = bits - 1; i >= 0; i--) {
        setPhaseBit((code >> i) & 0x1);
        delay(PSK31_BIT_DELAY);
    }
    // two zeros between characters
    setPhaseBit(0);
    delay(PSK31_BIT_DELAY * 2);
}

struct VC { char c; uint16_t code; uint8_t bits; };

// Varicode table for printable ASCII characters (32..126) using the standard
// PSK31 assignments. Each entry stores the binary varicode and the bit length.
static const VC table[] = {
    {' ', 0b1, 1},                  // space
    {'!', 0b111111111, 9},
    {'"', 0b101011111, 9},
    {'#', 0b111110101, 9},
    {'$', 0b111011011, 9},
    {'%', 0b1011010101, 10},
    {'&', 0b1010111011, 10},
    {'\'', 0b101111111, 9},
    {'(', 0b11111011, 8},
    {')', 0b11110111, 8},
    {'*', 0b101101111, 9},
    {'+', 0b111011111, 9},
    {',', 0b1110101, 7},
    {'-', 0b110101, 6},
    {'.', 0b1010111, 7},
    {'/', 0b110101111, 9},
    {'0', 0b10110111, 8},
    {'1', 0b10111101, 8},
    {'2', 0b11101101, 8},
    {'3', 0b11111111, 8},
    {'4', 0b101110111, 9},
    {'5', 0b101011011, 9},
    {'6', 0b101101011, 9},
    {'7', 0b110101101, 9},
    {'8', 0b110101011, 9},
    {'9', 0b110110111, 9},
    {':', 0b11110101, 8},
    {';', 0b110111101, 9},
    {'<', 0b111101101, 9},
    {'=', 0b1010101, 7},
    {'>', 0b111010111, 9},
    {'?', 0b1010101111, 10},
    {'@', 0b1010111101, 10},
    {'A', 0b1111101, 7},
    {'B', 0b11101011, 8},
    {'C', 0b10101101, 8},
    {'D', 0b10110101, 8},
    {'E', 0b1110111, 7},
    {'F', 0b11011011, 8},
    {'G', 0b11111101, 8},
    {'H', 0b101010101, 9},
    {'I', 0b1111111, 7},
    {'J', 0b111111101, 9},
    {'K', 0b101111101, 9},
    {'L', 0b11010111, 8},
    {'M', 0b10111011, 8},
    {'N', 0b11011101, 8},
    {'O', 0b10101011, 8},
    {'P', 0b11010101, 8},
    {'Q', 0b111011101, 9},
    {'R', 0b10101111, 8},
    {'S', 0b1101111, 7},
    {'T', 0b1101101, 7},
    {'U', 0b101010111, 9},
    {'V', 0b110110101, 9},
    {'W', 0b101011101, 9},
    {'X', 0b101110101, 9},
    {'Y', 0b101111011, 9},
    {'Z', 0b1010101101, 10},
    {'[', 0b111110111, 9},
    {'\\', 0b111101111, 9},
    {']', 0b111111011, 9},
    {'^', 0b1010111111, 10},
    {'_', 0b101101101, 9},
    {'`', 0b1011011111, 10},
    {'a', 0b1011, 4},
    {'b', 0b1011111, 7},
    {'c', 0b101111, 6},
    {'d', 0b101101, 6},
    {'e', 0b11, 2},
    {'f', 0b111101, 6},
    {'g', 0b1011011, 7},
    {'h', 0b101011, 6},
    {'i', 0b1101, 4},
    {'j', 0b111101011, 9},
    {'k', 0b1111111011, 10},
    {'l', 0b11011, 5},
    {'m', 0b111011, 6},
    {'n', 0b1111, 4},
    {'o', 0b111, 3},
    {'p', 0b1011101, 7},
    {'q', 0b1111111, 7},
    {'r', 0b10101, 5},
    {'s', 0b10111, 5},
    {'t', 0b101, 3},
    {'u', 0b110111, 6},
    {'v', 0b1110111, 7},
    {'w', 0b111111, 6},
    {'x', 0b1101101, 7},
    {'y', 0b1111011, 7},
    {'z', 0b11011111, 8},
    {'{', 0b111101011, 9},
    {'|', 0b1101111011, 10},
    {'}', 0b101011110, 9},
    {'~', 0b101011011, 9},
};

void PSK31::sendPSK31Message(const char *message) {
    for (const char *p = message; *p; ++p) {
        char c = *p;
        for (const auto &entry : table) {
            if (entry.c == c) {
                sendVaricode(entry.code, entry.bits);
                break;
            }
        }
    }
    _oscillator->setFrequency(0);
}
