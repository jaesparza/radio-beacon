
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
CW_SENDER *messenger;
FSK_SENDER *fsk_messenger;

uint32_t freq = 0;

#define CALIBRATION 100      // [Hz]
#define _30_M       10000000 // = 10MHz

#define QRSS_MESSAGE "EA2ECV"
#define CW_MESSAGE   "EA2ECV EA2ECV QTH COPENHAGEN DK 73"

void setup() {

    BEACON_SERIAL.begin(115200);

    oscillator = new AD9850(SPI_N, SPI_CLOCK, SELECT, RESET, FQUP);
    oscillator->init();
    oscillator->setCalibration(CALIBRATION);
    oscillator->setFrequency(_30_M);

    messenger = new CW_SENDER(oscillator);
    messenger->setBaseFrequency(_30_M);

    fsk_messenger = new FSK_SENDER(oscillator);
    fsk_messenger->setBaseFrequency(_30_M);

    BEACON_SERIAL.println("QRSS/WSPR Beacon initialized");
}

void loop() {
    messenger->txMessage(CW_MESSAGE);
    delay(2000);
    fsk_messenger->txMessage(QRSS_MESSAGE);
    delay(2000);
}
