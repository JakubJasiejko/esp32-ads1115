#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>

// === Adres wskaźników (pointer register) ===
#define ADS1115_POINTER_CONVERSION 0x00
#define ADS1115_POINTER_CONFIG     0x01

// === MUX - konfiguracje pomiarowe ===
#define ADS1115_DIFF_0_1           0b000
#define ADS1115_DIFF_0_3           0b001
#define ADS1115_DIFF_1_3           0b010
#define ADS1115_DIFF_2_3           0b011
#define ADS1115_SINGLE_0           0b100
#define ADS1115_SINGLE_1           0b101
#define ADS1115_SINGLE_2           0b110
#define ADS1115_SINGLE_3           0b111

// === PGA - zakresy napięcia ===
#define ADS1115_PGA_6_144V         0b000
#define ADS1115_PGA_4_096V         0b001
#define ADS1115_PGA_2_048V         0b010
#define ADS1115_PGA_1_024V         0b011
#define ADS1115_PGA_0_512V         0b100
#define ADS1115_PGA_0_256V         0b101

// === Tryb pracy ===
#define ADS1115_MODE_CONTINUOUS    0b0
#define ADS1115_MODE_SINGLE        0b1

// === Częstotliwości próbkowania (data rate) ===
#define ADS1115_RATE_8SPS          0b000
#define ADS1115_RATE_16SPS         0b001
#define ADS1115_RATE_32SPS         0b010
#define ADS1115_RATE_64SPS         0b011
#define ADS1115_RATE_128SPS        0b100
#define ADS1115_RATE_250SPS        0b101
#define ADS1115_RATE_475SPS        0b110
#define ADS1115_RATE_860SPS        0b111

// === Funkcje ===
uint8_t  ADS1115_measure(uint8_t address, uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dataRate);
int16_t  ADS1115_read_raw(uint8_t address, uint64_t timeout_us);
float    ADS1115_read_mV(uint8_t address, uint64_t timeout_us, uint8_t pga_code);

#endif // ADS1115_H
