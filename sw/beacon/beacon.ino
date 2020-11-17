
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
uint32_t calibration = 96;

void setup() {

    BEACON_SERIAL.begin(115200);
    oscillator = new AD9850(SPI_N, SPI_CLOCK, SELECT, RESET, FQUP);
    oscillator->init();
    oscillator->setFrequency(10000000);

    messenger = new QRSS(oscillator);
    messenger->setBaseFrequency(10000000);

    BEACON_SERIAL.println("QRSS/WSPR Beacon initialized");
}

void loop() {
    // messenger->generateTestSequence(1000, 10000000, 10);
    // messenger->generateTestWSPR();
    messenger->txMessage("CQ CQ DE EA2ECV");
    delay(1000);
}
