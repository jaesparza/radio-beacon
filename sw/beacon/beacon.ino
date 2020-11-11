
/*
 * Filename: beacon.ino
 * Description: main file for the beacon controller.
 * Author: jaesparza - jaesparza@gmail.com
 *
 * Compiled with arduino IDE version 1.8.10, edited with VScode and formatted
 * with with clang-format.
 *
 * Project: radio-beacon
 * Hardware schematics: See HW folder in Github
 *
 */

#include "./src/lib/AD9850.h"
#include "HardwareConfig.h"
#include <Arduino.h>

AD9850 oscillator;

uint32_t freq = 0;

void setup() {
    BEACON_SERIAL.begin(115200);
    initialized();
    oscillator.begin(SELECT, RESET, FQUP);
    freq = 10000000;
    oscillator.setFrequency(freq);
}

void loop() {}

void initialized() {
    BEACON_SERIAL.println("QRSS/WSPR Beacon init");
}

void testToneGeneration() {
    freq = 10140250 + 1.468;
    delay(1000);
    oscillator.setFrequency(freq);
    freq = freq + 1.468;
    delay(1000);
    oscillator.setFrequency(freq);
    freq = freq + 1.468;
    delay(1000);
    oscillator.setFrequency(freq);
}
