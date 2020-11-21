# radio-beacon

Radio beacon for QRSS and WSPR modes, based on a STM32 and an AD9850.

## Puting the beacon on the air

The beacon can be configured with a custom message, TX frequency, shift and transmission time. The process is as follows:

1. Define the message to be sent under the desired mode.
2. Set the band and the shift around the transmission frequency.
3. Set the delay between transmissions in seconds.
4. Beacon will transmit periodically.

```C
#define CW_MESSAGE   "OZ/EA2ECV OZ/EA2ECV QTH COPENHAGEN DK 73"
#define _30_M_QRSS 10140000 // 10MHz
#define SHIFT 0 // [Hz]
```

For FSK-CW transmission, the process is the same. A separate message will have to be defined:
```C
#define QRSS_MESSAGE "CQ OZ/EA2ECV"
```
Additionally, the timing can be configured through the following definitions in the QRSS.h file. It is a must to configure it to suit your needs.
```C
#define QRSS_DOT    2000 // Duration of a dot in milliseconds
#define QRSS_WEIGHT 3 // Dash-dot weight, DASH duration = QRSS_DOT * QRSS_WEIGHT
#define QRSS_DELAY  2000 // Time delay after keying a letter

#define FSK_HIGH 10 // Frequency shift used in the keying 
```
The snapshot below shows the start of `QRSS_MESSAGE` being transmmited in FSK-CW as captured locally by Argo. This transmission was performed without an amplifier, coming directly from a wire connected to the AD9850 oscillator.

![fsk](/doc/images/cqFSKCW.png)