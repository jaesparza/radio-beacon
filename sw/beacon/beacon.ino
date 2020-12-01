
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
TinyGPSPlus *gps;
RTClock rt;

uint32_t freq = 0;
tm_t mtt;

#define CALIBRATION                                                            \
    0 // 100 [Hz] Hardcoded calibration factor for the oscillator (will vary
      // across samples)

#define _30_M_QRSS 10140000 // 10MHz
#define _20_M_QRSS 14096900 // 14MHz

// Dial freq     | TX freq
// 20m 14.095600 | 14.097000 - 14.097200
// 30m 10.138700 | 10.140100 - 10.140300
#define _30_M_WSPR 10140100
#define _20_M_WSPR 14097000

#define _10MHZ 10000000 // 10MHz target calibration

#define SHIFT                                                                  \
    0 // [Hz] Hardcoded eviation around the center QRSS frequency. Based on the
      // current bandplan in can range from -100 to +100 Hz

// Default messages - ALWAYS in uppercase. Valid characters: A to Z (standard
// english alphabet), 0 to 9 and /
#define QRSS_MESSAGE "OZ/EA2ECV"
#define CW_MESSAGE   "OZ/EA2ECV OZ/EA2ECV QTH COPENHAGEN DK 73"

char s[128];

extern const char *weekdays[];
extern const char *months[];

void setup() {

    // Serial output can be activated if needed
    BEACON_SERIAL.begin(115200);
    GPS_SERIAL.begin(9600);

    // Create an oscillator instance, calibrate it and set initial frequency
    oscillator = new AD9850(SPI_N, SPI_CLOCK, PIN_SELECT, PIN_RESET, PIN_FQUP);
    oscillator->init();
    oscillator->setCalibration(CALIBRATION);

    /*
        // Uncomment to calibrate in 30MHz. Modify as needed to calibrate other
        // bands
        oscillator->setFrequency(_10MHZ);
    */

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
    rt = *(new RTClock(RTCSEL_LSE));

    BEACON_SERIAL.println("--> QRSS/WSPR Beacon initialized");

    syncRTC();

    rt.attachAlarmInterrupt(beaconTX);
    rt.setAlarmTime(rt.getTime() + 10);
    BEACON_SERIAL.println("--> RTC synchronized to GPS time");
}

void loop() {

    // wsprSender->sendWSPRmessage();

    /* // Test code to monitor the GPS or the RTC
       // Select one function
      NMEAstringGrabber(); // Prints all the incomping NMEA strings
      getGPStime(); // Prints the hour after filtering NMEA strings
     */
    monitorRTC();

    /* // Test code for the CW mode
        messenger->txMessage(CW_MESSAGE);
        delay(2000);
    */

    /* // Test code for the QRSS mode
        fskMessenger->txMessage(QRSS_MESSAGE);
        delay(2000);
    */
}

void beaconTX() {
    BEACON_SERIAL.println(" Interrupt fired ");
    rt.setAlarmTime(rt.getTime() + 10);
}

uint8_t syncRTC() {
    bool timeUpdatePending = true;
    int i = 0;
    while (timeUpdatePending) {
        i++;
        while (GPS_SERIAL.available() > 0) {
            gps->encode(GPS_SERIAL.read());
            if (gps->time.isUpdated()) {
                mtt.month = gps->date.month();
                mtt.day = gps->date.day();
                mtt.year = gps->date.year() - 1970;
                mtt.hour = gps->time.hour();
                mtt.minute = gps->time.minute();
                mtt.second = gps->time.second();
                rt.setTime(rt.makeTime(mtt));

                timeUpdatePending = false;
            }
        }
    }

#ifdef _DEBUG_OUT
    BEACON_SERIAL.print("YEAR ");
    BEACON_SERIAL.print(gps->date.year());
    BEACON_SERIAL.print(" MONTH ");
    BEACON_SERIAL.print(gps->date.month());
    BEACON_SERIAL.print(" DAY ");
    BEACON_SERIAL.print(gps->date.day());
    BEACON_SERIAL.print("UTC time: ");
    BEACON_SERIAL.print(gps->time.hour()); // Hour (0-23) (u8)
    BEACON_SERIAL.print(":");
    BEACON_SERIAL.print(gps->time.minute()); // Minute (0-59) (u8)
    BEACON_SERIAL.print(":");
    BEACON_SERIAL.print(gps->time.second()); // Second (0-59) (u8)
    BEACON_SERIAL.println();
    BEACON_SERIAL.print("Attempts until time read: ");
    BEACON_SERIAL.println(i);
#endif //_DEBUG_OUT
}

void NMEAstringGrabber() {
    while (1) {
        while (GPS_SERIAL.available() > 0) {
            BEACON_SERIAL.write(GPS_SERIAL.read());
        }
    }
}

void getGPStime() {
    while (1) {
        while (GPS_SERIAL.available() > 0) {
            gps->encode(GPS_SERIAL.read());
            if (gps->time.isUpdated()) {
                BEACON_SERIAL.print("UTC time: ");
                BEACON_SERIAL.print(gps->time.hour()); // Hour (0-23) (u8)
                BEACON_SERIAL.print(":");
                BEACON_SERIAL.print(gps->time.minute()); // Minute (0-59) (u8)
                BEACON_SERIAL.print(":");
                BEACON_SERIAL.print(gps->time.second()); // Second (0-59) (u8)
                BEACON_SERIAL.println();
            }
        }
    }
}

void monitorRTC() {

    static uint8_t lastSecond = 0;

    rt.breakTime(rt.now(), mtt);

    if (mtt.second != lastSecond) {
        lastSecond = mtt.second;
        sprintf(s, "RTC timestamp: %s %u %u, %s, %02u:%02u:%02u\n",
                months[mtt.month], mtt.day, mtt.year + 1970,
                weekdays[mtt.weekday], mtt.hour, mtt.minute, mtt.second);
        BEACON_SERIAL.print(s);
    }
}