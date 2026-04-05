/**
 * @file ads1115.c
 * @brief ADS1115 helper implementation for ESP32.
 */

#include "ads1115.h"
#include "i2c.h"
#include "uart.h"

/**
 * @brief Starts an ADS1115 conversion using the requested configuration bits.
 *
 * @param address I2C device address
 * @param os Operational status / single-shot trigger bit
 * @param mux Input multiplexer configuration
 * @param pga Full-scale range selection
 * @param mode Conversion mode
 * @param dataRate Output data rate
 * @return 0x00 on success, 0xFF on error
 */
uint8_t ADS1115_measure(uint8_t address, uint8_t os, uint8_t mux, uint8_t pga, uint8_t mode, uint8_t dataRate) {
    uint16_t config = (os << 15) | (mux << 12) | (pga << 9) | (mode << 8) | (dataRate << 5) | 0b00011;
    if (i2c_write16(address, ADS1115_POINTER_CONFIG, config) != 0x0000) {
        uart_write_bytes(UART_PORT, "ADS1115 init error\r\n", 21);
        return 0xFF;
    }
    return 0x00;
}

/**
 * @brief Waits for conversion completion and returns the raw ADC code.
 *
 * @param address I2C device address
 * @param timeout_us Timeout in microseconds
 * @return Signed 16-bit conversion result, or 0x7FFF on timeout/error
 */
int16_t ADS1115_read_raw(uint8_t address, uint64_t timeout_us) {
    int64_t start_time = esp_timer_get_time();

    while ((esp_timer_get_time() - start_time) < timeout_us) {
        uint16_t config = i2c_read16(address, ADS1115_POINTER_CONFIG);
        if (config == 0xFFFF) continue;  // Read error

        // Check the OS bit (bit 15). When set, the conversion is ready.
        if ((config & 0x8000) != 0) {
            uint16_t raw = i2c_read16(address, ADS1115_POINTER_CONVERSION);
            return (int16_t)raw;  // Signed 16-bit result
        }

        esp_rom_delay_us(500);  // Wait briefly and poll again
    }

    // Timeout: return a sentinel error value.
    return 0x7FFF;
}

/**
 * @brief Converts the ADS1115 raw result to millivolts.
 *
 * @param address I2C device address
 * @param timeout_us Timeout in microseconds
 * @param pga_code PGA full-scale range code
 * @return Converted value in millivolts, or 99999.0f on error
 */
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
        default: return 99999.0f; // Unknown PGA code
    }

    return (float)raw * lsb_mv;
}
