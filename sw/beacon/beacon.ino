
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

#include "./src/QRSS.h"
#include "./src/lib/AD9850.h"
#include "HardwareConfig.h"
#include <Arduino.h>

AD9850 *oscillator;
QRSS *messenger;

uint32_t freq = 0;

void setup() {

    BEACON_SERIAL.begin(115200);
    oscillator = new AD9850(SPI_N, SPI_CLOCK, SELECT, RESET, FQUP);
    oscillator->init();

    messenger = new QRSS(oscillator);
    freq = 10000000;
    oscillator->setFrequency(freq);

    BEACON_SERIAL.println("QRSS/WSPR Beacon initialized");
}

void loop() {}

void testToneGeneration() {
    freq = 10140250 + 1.468;
    delay(1000);
    oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(1000);
    oscillator->setFrequency(freq);
    freq = freq + 1.468;
    delay(1000);
    oscillator->setFrequency(freq);
}
