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
#include "./src/TimeSync.h"
#include "./src/WSPR.h"
#include "./src/lib/AD9850.h"
#include "./src/lib/TinyGPSPlus.h"

#include "HardwareConfig.h"

#include <RTClock.h>

AD9850 *oscillator;
CW_SENDER *messenger;
FSK_SENDER *fskMessenger;
WSPR *wsprSender;
TimeSync *timeKeeper;

uint32_t freq = 0;

TinyGPSPlus *gps;
RTClock *rt;

#define CALIBRATION                                                            \
    100 // 100 [Hz] Hardcoded calibration factor for the oscillator (will vary
        // across samples)

#define _30_M_QRSS 10140000 // 10MHz
#define _20_M_QRSS 14096900 // 14MHz

// Dial freq     | TX freq
// 20m 14.095600 | 14.097000 - 14.097200
// 30m 10.138700 | 10.140100 - 10.140300
#define _30_M_WSPR 10140100
#define _20_M_WSPR 14097100

#define _10MHZ 10000000 // 10MHz target calibration

#define SHIFT                                                                  \
    0 // [Hz] Hardcoded eviation around the center QRSS frequency. Based on the
      // current bandplan in can range from -100 to +100 Hz

// Default messages - ALWAYS in uppercase. Valid characters: A to Z (standard
// english alphabet), 0 to 9 and /
#define QRSS_MESSAGE "OZ/EA2ECV"
#define CW_MESSAGE   "OZ/EA2ECV OZ/EA2ECV QTH COPENHAGEN DK 73"

// Will trigger a transmission in the next time slot
#define SCHEDULE_FOR_NEXT_SLOT 0

// Will trigger a tranmission in the next timeslot after 5 minutes
#define SCHEDULE_TX_PERIODIC 5

#define _CALIBRATE 0

void setup() {

    // Serial output can be activated if needed
    BEACON_SERIAL.begin(115200);
    GPS_SERIAL.begin(9600);

    // Create an oscillator instance, calibrate it and set initial frequency
    oscillator = new AD9850(SPI_N, SPI_CLOCK, PIN_SELECT, PIN_RESET, PIN_FQUP);
    oscillator->init();
    oscillator->setCalibration(CALIBRATION);

    if (_CALIBRATE) {
        // Uncomment to calibrate in the selected band
        oscillator->setFrequency(_20_M_WSPR);
        BEACON_SERIAL.println("Calibration mode ON");
        while (1) {
        }
    }

    /*
        // Create a CW sender and tune it
        messenger = new CW_SENDER(oscillator);
        messenger->setBaseFrequency(_30_M_QRSS + SHIFT);

        // Create a fsk cw sender and tune it
        fskMessenger = new FSK_SENDER(oscillator);
        fskMessenger->setBaseFrequency(_30_M_QRSS + SHIFT);
    */

    // Create a WSPR sender and tune it
    wsprSender = new WSPR(oscillator);
    wsprSender->setBaseFrequency(_20_M_WSPR);
    gps = new TinyGPSPlus();
    rt = new RTClock(RTCSEL_LSE);

    timeKeeper = new TimeSync(rt, gps);

    BEACON_SERIAL.println("--> QRSS/WSPR Beacon initialized");

    timeKeeper->syncRTC();
    timeKeeper->scheduleNextWSPRTX(beaconTX, SCHEDULE_FOR_NEXT_SLOT);

    BEACON_SERIAL.println("--> RTC synchronized to GPS time");
}

volatile boolean TX_triggered = false;

void loop() {

    // The txMessages functions will block until the whole message has been
    // transmitted, nothing else will run during that process.

    // timeKeeper->monitorRTC();

    if (TX_triggered) {
        // timeKeeper->monitorRTC();
        BEACON_SERIAL.println("Beacon sending WSPR frame");
        wsprSender->sendWSPRmessage();

        // Test code for the CW mode
        // messenger->txMessage(CW_MESSAGE);

        // Test code for the QRSS mode
        // fskMessenger->txMessage(QRSS_MESSAGE);

        timeKeeper->scheduleNextWSPRTX(beaconTX, SCHEDULE_TX_PERIODIC);
        TX_triggered = false;
    }
}

/*
 Executed from an interrupt context, so it only signals the main loop
 through a volatile variable that Tx has to begin.
*/
void beaconTX() {
    TX_triggered = true;
}