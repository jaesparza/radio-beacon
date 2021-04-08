
## File overview
The software is organized as described below. This folder structure is required to compile with the arduino environment (compiled with 1.8.10).

```bash
beacon/
|── beacon.ino              ## Initialization and entry point
├── HardwareConfig.h        ## Configuration of hardware connection
├── src/                                    
│   ├── lib               
|   |   ├── TinyGPSPlus.cpp  ## Library to process NMEA strings
|   |   ├── TinyGPSPlus.h
|   |   ├── AD9850.h        ## Library to interface the AD9850 DDS
|   |   └── AD9850.cpp
│   ├── QRSS.h              ## QRSS base class and specialized FSK-CW and CW subclasses
|   |── QRSS.cpp
|   |── WSPR.h              ## WSPR transmission
|   |── UWSPR.cpp
|   |── TimeSync.h          ## Class to handle RTC to GPS time synchronization and alarms
|   |── TimeSync.cpp
|   └── 
├── README.md               ## This file
└── 
```

## Configuration
Make sure to set the correct hardware configuration in `HardwareConfig.h` depending on which interfaces are in use. If the software is compiled for an board different than an STM32 (blue-pill "edition"), pins might need remapping.

```C
// Selected digital IOs
#define PIN_SELECT 4
#define PIN_RESET  25
#define PIN_FQUP   24

// Selected Serial and SPI interfaces
#define BEACON_SERIAL Serial1
#define GPS_SERIAL    Serial3
#define SPI_N         2
#define _5_5_COMPATIBLE
```

## Test functions

The system can use the timeKeepr class to monitor the gps time, get the NMEA strings from GPS and also get the RTC time.
```C
// Test code to monitor the GPS or the RTC
timeKeeper->getGPStime();           // Prints the hour after filtering NMEA strings
timeKeeper->NMEAstringGrabber();    // Prints all the incomping NMEA strings
timeKeeper->monitorRTC();           // Prints out RTC time
```

The oscillator can be tested by generating WSPR test tones sequentially. This can be useful to test the tone separation.
```C
/*
 * This function will generate a sequence of test tones that can be used to
 * verify the correct operation of the oscillator at WSPR frequencies before
 * sending a complete message.
 */
void WSPR::generateTestTones() {
    for (uint8_t i = 0; i < 4; i++) {
        generateTone(i);
        delay(5000);
    }
}
```

## Configure your own WSPR frame
Your custom WSPR frame shall be provided in a `uint8_t` array to the WSPR class. This is defined now at compile-time. To generate your own frame under windows, you can use WSPRcode. In the example below AB2CDE is the callsing, XX99 the 4 digit locator and 00 the tx power in dbm.

To convert watts into dbm in a 50 ohms system, you can use an online calculator, a conversion table, etc...

 ```
 c:\> WSPRcode "AB2CDE XX99 00"
 ```


```C
/*
 * Example message from the WSPR user manual
 *
 * The standard format according to the protocol spec. is: "callsign + 4-digit
 * locator + dBm"
 * Channel symbols for the message "K1ABC FN42 37"
 */
uint8_t EXAMPLE_WSPR_DATA[] = {
    3, 3, 0, 0, 2, 0, 0, 0, 1, 0, 2, 0, 1, 3, 1, 2, 2, 2, 1, 0, 0, 3, 2, 3,
    1, 3, 3, 2, 2, 0, 2, 0, 0, 0, 3, 2, 0, 1, 2, 3, 2, 2, 0, 0, 2, 2, 3, 2,
    1, 1, 0, 2, 3, 3, 2, 1, 0, 2, 2, 1, 3, 2, 1, 2, 2, 2, 0, 3, 3, 0, 3, 0,
    3, 0, 1, 2, 1, 0, 2, 1, 2, 0, 3, 2, 1, 3, 2, 0, 0, 3, 3, 2, 3, 0, 3, 2,
    2, 0, 3, 0, 2, 0, 2, 0, 1, 0, 2, 3, 0, 2, 1, 1, 1, 2, 3, 3, 0, 2, 3, 1,
    2, 1, 2, 2, 2, 1, 3, 3, 2, 0, 0, 0, 0, 1, 0, 3, 2, 0, 1, 3, 2, 2, 2, 2,
    2, 0, 2, 3, 3, 2, 3, 2, 3, 3, 2, 0, 0, 3, 1, 2, 2, 2,
};
```


## Compilation

In my setup, I use vscode with the arduino plug-in configured as follows for a STM32 "blue-pill" board. This configuration uses the core definition from Roger Clark [GitHub](https://github.com/rogerclarkmelbourne/Arduino_STM32).
```json
{
    "board": "stm32duino:STM32F1:genericSTM32F103C",
    "configuration": "device_variant=STM32F103C8,upload_method=STLinkMethod,cpu_speed=speed_72mhz,opt=osstd",
    "port": "COM16",
    "sketch": "sw\\beacon\\beacon.ino"
}
```

The following configuration uses the STM32 official core definition - which cannot handle the second SPI interface at the moment.
```json
{
    "board": "STM32:stm32:GenF1",
    "configuration": "pnum=BLUEPILL_F103C6,upload_method=swdMethod,xserial=generic,usb=none,xusb=FS,opt=osstd,rtlib=nano",
    "port": "COM16",
    "sketch": "sw\\beacon\\beacon.ino"
}
```