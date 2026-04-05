# esp32-ads1115

Simple `ADS1115` driver for ESP32 over I2C. It is a good fit for quick single-ended and differential measurement prototypes.

## Features
- write measurement configuration
- read raw samples
- convert samples to millivolts

## Dependencies
- `esp32-i2c-tools`
- optional `esp32-uart-xor` for debug output

## Files
- `include/ads1115.h`
- `src/ads1115.c`

## Quick start
```c
#include "ads1115.h"

void app_main(void)
{
    initI2C();
    ADS1115_measure(0x48, 1, ADS1115_SINGLE_0, ADS1115_PGA_4_096V, ADS1115_MODE_SINGLE, ADS1115_RATE_128SPS);
    float mv = ADS1115_read_mV(0x48, 50000, ADS1115_PGA_4_096V);
    (void)mv;
}
```

See `examples/basic_usage/basic_usage.c` for a complete example.
