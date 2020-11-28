
## File overview

The software is organized as described below. This folder structure is required to compile with the arduino environment (compiled with 1.8.10).

```bash
beacon/
|── beacon.ino            ## Initialization and entry point
├── HardwareConfig.h      ## Configuration of hardware connection
├── src/                                    
│   ├── lib               ## Library to interface the AD9850 DDS
|   |   ├── AD9850.h
|   |   └── AD9850.cpp
│   ├── QRSS.h            ## QRSS base class and specialized FSK-CW and CW subclasses
|   |── QRSS.cpp
|   |── WSPR.h            ## WSPR transmission
|   |── UWSPR.cpp
|   └── 
└── 
```

## Configuration
Make sure to set the correct hardware configuration in `HardwareConfig.h` depending on which interfaces are in use. If the software is compiled for an board different than an STM32 (blue-pill "edition"), pins might need remapping.

```C
// Selected digital IOs
#define SELECT 4
#define RESET_PIN  25
#define FQUP   24

// Selected Serial and SPI interfaces
#define BEACON_SERIAL Serial1
#define SPI_N         2
#define _5_5_COMPATIBLE
```

## Compilation

In my setup, I use vscode with the arduino plug-in configured as follows for a STM32 "blue-pill" board:
```json
{
    "board": "stm32duino:STM32F1:genericSTM32F103C",
    "configuration": "device_variant=STM32F103C8,upload_method=STLinkMethod,cpu_speed=speed_72mhz,opt=osstd",
    "port": "COM16",
    "sketch": "sw\\beacon\\beacon.ino"
}
```

The following configuration uses an alternative core definition. Remains to be tested.
```json
{
    "board": "STM32:stm32:GenF1",
    "configuration": "pnum=BLUEPILL_F103C6,upload_method=swdMethod,xserial=generic,usb=none,xusb=FS,opt=osstd,rtlib=nano",
    "port": "COM16",
    "sketch": "sw\\beacon\\beacon.ino"
}
```