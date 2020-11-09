

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#define SELECT 4
#define RESET  25
#define FQUP   24

#define BEACON_SERIAL Serial1
#define SPI_N         2

//#define SPI1 myInterface

//#define _5_5_COMPATIBLE

#ifdef _3_3_V
#define SPI_CLOCK 5
#define SPI_MISO  6
#define SPI_MOSI  7
#endif

#ifdef _5_5_COMPATIBLE
#define SPI_CLOCK PB13 // 29
#define SPI_MISO  PB14 // 30
#define SPI_MOSI  PB15 // 31
#endif

#define SPI_CLOCK PB13 // 29

#endif /* HARDWARECONFIG_H_ */