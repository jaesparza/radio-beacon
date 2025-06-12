# Adding PSK31 Support

The beacon already implements WSPR and QRSS using the AD9850 DDS. The same hardware can be used for PSK31 since the driver exposes phase control through `AD9850::setPhase()`.

## Approach

1. **Symbol Rate**: PSK31 runs at 31.25 baud. Use a bit period of roughly 32&nbsp;ms (`delay(32)`).
2. **Phase Changes**: BPSK31 toggles the carrier by 180°. Use `setPhase(0)` for a `0` and `setPhase(16)` for a `1` (16 × 11.25° ≈ 180°).
3. **Varicode**: Characters are encoded using varicode. Implement a lookup table where each entry stores the bit pattern and its length. After transmitting a character append two zero bits as a separator.
4. **PSK31 Class**: Create a `PSK31` class mirroring `WSPR` and `QRSS`:
   ```C++
   class PSK31 {
   public:
       PSK31(AD9850 *osc);
       void setBaseFrequency(uint32_t base);
       void sendMessage(const char *msg);
   };
   ```
5. **Raised Cosine Envelope**: For cleaner transmissions, optionally shape the phase transitions using a raised‑cosine function rather than abrupt steps.

Implementing the points above allows the beacon to transmit text messages using the PSK31 mode.
