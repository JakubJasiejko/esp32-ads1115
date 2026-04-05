# esp32-ads1115

Prosty sterownik `ADS1115` dla ESP32 przez I2C. Nadaje się do szybkiego startu z pomiarem single-ended i differential.

## Co zawiera
- zapis konfiguracji pomiaru
- odczyt surowej próbki
- konwersję na miliwolty

## Zależności
- `esp32-i2c-tools`
- opcjonalnie `esp32-uart-xor` dla debugowania

## Pliki
- `include/ads1115.h`
- `src/ads1115.c`

## Szybki start
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

Pełny przykład znajdziesz w `examples/basic_usage/basic_usage.c`.
