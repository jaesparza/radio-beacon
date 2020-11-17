

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#define SELECT 4
#define RESET  25
#define FQUP   24

#define BEACON_SERIAL Serial1
#define SPI_N         2

#define _5_5_COMPATIBLE

#ifdef _3_3_V
#define SPI_CLOCK PA5 // (5)
// SPI_MISO  PA6 (6) , SPI_MOSI  PA7 (7)
#endif

#ifdef _5_5_COMPATIBLE
#define SPI_CLOCK PB13 // (29)
// SPI_MISO  PB14(30) SPI_MOSI  PB15(31)
#endif

#endif /* HARDWARECONFIG_H_ */