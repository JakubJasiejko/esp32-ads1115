#include "ads1115.h"
#include "i2c.h"
#include "uart.h"

/**
 * @brief Inicjalizuje ADS1115 (konfiguracja pomiaru różnicowego AIN0-AIN1).
 */
uint8_t ADS1115_measure(uint8_t address, uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dataRate) {
    uint16_t config = (os << 15) | (mux << 12) | (pga << 9) | (mode << 8) | (dataRate << 5) | 0b00011;
    if (i2c_write16(address, ADS1115_POINTER_CONFIG, config) != 0x0000) {
        uart_write_bytes(UART_PORT, "ADS1115 init error\r\n", 21);
        return 0xFF;
    }
    return 0x00;
}

int16_t ADS1115_read_raw(uint8_t address, uint64_t timeout_us) {
    int64_t start_time = esp_timer_get_time();

    while ((esp_timer_get_time() - start_time) < timeout_us) {
        uint16_t config = i2c_read16(address, ADS1115_POINTER_CONFIG);
        if (config == 0xFFFF) continue;  // błąd odczytu

        // Sprawdź bit OS (bit 15) – jeśli ustawiony, pomiar gotowy
        if ((config & 0x8000) != 0) {   //binarny and 0b0 i 0b1 = 0b0, ob1 i 0b1 = 0b1
            uint16_t raw = i2c_read16(address, ADS1115_POINTER_CONVERSION);
            return (int16_t)raw;  // dane są w formacie signed 16-bit
        }

        esp_rom_delay_us(500);  // odczekaj chwilę i spróbuj ponownie
    }

    // Timeout – zwracamy wartość błędną
    return 0x7FFF;
}

float ADS1115_read_mV(uint8_t address, uint64_t timeout_us, uint8_t pga_code) {
    int16_t raw = ADS1115_read_raw(address, timeout_us);
    if (raw == 0x7FFF) return 99999.0f;

    float lsb_mv;
    switch (pga_code) {
        case ADS1115_PGA_6_144V: lsb_mv = 0.1875f; break;
        case ADS1115_PGA_4_096V: lsb_mv = 0.1250f; break;
        case ADS1115_PGA_2_048V: lsb_mv = 0.0625f; break;
        case ADS1115_PGA_1_024V: lsb_mv = 0.03125f; break;
        case ADS1115_PGA_0_512V: lsb_mv = 0.015625f; break;
        case ADS1115_PGA_0_256V: lsb_mv = 0.0078125f; break;
        default: return 99999.0f; // Nieznany kod PGA
    }

    return (float)raw * lsb_mv;
}
