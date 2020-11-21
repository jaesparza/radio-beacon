
/*
 * Filename: beacon.ino
 * Description: main file for the beacon controller.
 * Author: ja - jaesparza@gmail.com
 *
 * Compiled with arduino IDE version 1.8.10, edited with VScode and formatted
 * with with clang-format.
 *
 * Project: radio-beacon
 * Hardware schematics: See HW folder in Github
 *
 */

#include "./src/QRSS.h"
#include "./src/WSPR.h"
#include "./src/lib/AD9850.h"

#include "HardwareConfig.h"
#include <Arduino.h>

AD9850 *oscillator;
CW_SENDER *messenger;
FSK_SENDER *fskMessenger;
WSPR *wsprSender;

uint32_t freq = 0;

#define CALIBRATION                                                            \
    100 // [Hz] Hardcoded calibration factor for the oscillator (will vary
        // across samples)

#define _30_M_QRSS 10140000 // 10MHz
#define _20_M_QRSS 14096900 // 14MHz

#define SHIFT                                                                  \
    0 // [Hz] Hardcoded eviation around the center QRSS frequency. Based on the
      // current bandplan in can range from -100 to +100 Hz

// Default messages - ALWAYS in uppercase. Valid characters: A to Z (standard
// english alphabet), 0 to 9 and /
#define QRSS_MESSAGE "OZ/EA2ECV"
#define CW_MESSAGE   "OZ/EA2ECV OZ/EA2ECV QTH COPENHAGEN DK 73"

#define WSPR_MESSAGE

void setup() {

    // Serial output can be activated if needed
    // BEACON_SERIAL.begin(115200);

    // Create an oscillator instance, calibrate it and set initial frequency
    oscillator = new AD9850(SPI_N, SPI_CLOCK, SELECT, RESET, FQUP);
    oscillator->init();
    oscillator->setCalibration(CALIBRATION);
    oscillator->setFrequency(_30_M_QRSS);

    // Create a CW sender for later use
    messenger = new CW_SENDER(oscillator);
    messenger->setBaseFrequency(_30_M_QRSS + SHIFT);

    // Create a fsk cw sender for later use
    fskMessenger = new FSK_SENDER(oscillator);
    fskMessenger->setBaseFrequency(_30_M_QRSS + SHIFT);

    wsprSender = new WSPR(oscillator);

    // BEACON_SERIAL.println("QRSS/WSPR Beacon initialized");
}

void loop() {
    messenger->txMessage(CW_MESSAGE);
    delay(2000);
    fskMessenger->txMessage(QRSS_MESSAGE);
    delay(2000);
}
