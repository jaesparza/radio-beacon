#include "./src/lib/AD9850.h"
#include "HardwareConfig.h"
#include <Arduino.h>

AD9850 oscillator;

uint32_t freq = 0;

void setup() {
    BEACON_SERIAL.begin(115200);
    initialized();
    oscillator.begin(SELECT, RESET, FQUP);
}

void loop() {

    /*
    freq =    10140250 + 1.468;
    delay(1000);
    freqGen.setFrequency(freq);
    freq = freq + 1.468;
    delay(1000);
    freqGen.setFrequency(freq);
    freq = freq + 1.468;
    delay(1000);
    freqGen.setFrequency(freq);
    */
    // freq =  20000000;
    freq = 10000000;
    oscillator.setFrequency(freq);
}

void initialized() {
    BEACON_SERIAL.println("QRSS Beacon");
}
