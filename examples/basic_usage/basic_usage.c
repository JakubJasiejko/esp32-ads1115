#include "ads1115.h"
#include "i2c.h"

void app_main(void)
{
    initI2C();
    ADS1115_measure(0x48, 1, ADS1115_SINGLE_0, ADS1115_PGA_4_096V, ADS1115_MODE_SINGLE, ADS1115_RATE_128SPS);
    float mv = ADS1115_read_mV(0x48, 50000, ADS1115_PGA_4_096V);
    (void)mv;
}
