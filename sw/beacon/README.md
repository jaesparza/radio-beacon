
## File overview

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
#define RESET  25
#define FQUP   24

// Selected Serial and SPI interfaces
#define BEACON_SERIAL Serial1
#define SPI_N         2
#define _5_5_COMPATIBLE
```