/* Copyright 2019 Raphael Randschau
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "oled.h"
#include "i2c_master.h"
#include "m24m01.h"
#include "print.h"
#include "debug.h"

// write address: 0xA0
// read address: 0x50
#define EEPROM_PAGESIZE        128

uint8_t init_m24m01(void) {
    palSetPadMode(M24M01_WC_PAD, M24M01_WC_PIN, PAL_MODE_OUTPUT_PUSHPULL);
    // palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);
    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);

    return i2c2_isDeviceReady(EEPROM_ADDRESS, EEPROM_LONG_TIMEOUT);
}

uint8_t m24m01_page_read(uint8_t address, uint8_t page, uint8_t*data, uint16_t length)
{
    uint8_t status = i2c2_receive(address, &data[0], length, EEPROM_LONG_TIMEOUT);
    i2c2_stop();

    return status;
}

uint8_t m24m01_page_write(uint8_t address, uint8_t page, const uint8_t*data)
{
    // pull down WC to allow writes
    palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);

    uint8_t status = i2c2_transmit((address<<1), &data[0], sizeof(data), EEPROM_LONG_TIMEOUT);
    i2c2_stop();

    // pull up WC to prevent writes
    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);
    return status;
}

uint8_t m24m01_random_byte_read(uint8_t address, uint8_t addr1, uint8_t addr2, uint8_t *data) {
    const uint8_t location[2] = {addr1, addr2};
    uint8_t status = i2c2_transmit((address<<1), &location[0], 2, EEPROM_LONG_TIMEOUT);
    status = i2c2_receive((address<<1)|1, data, 1, EEPROM_LONG_TIMEOUT);
    i2c2_stop();
    return status;
}

uint8_t m24m01_byte_write(uint8_t address, uint8_t addr1, uint8_t addr2, uint8_t data) {
    const uint8_t location_and_byte[3] = {addr1, addr2, data};
    palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);

    uint8_t status = i2c2_transmit(address<<1, &location_and_byte[0], 3, EEPROM_LONG_TIMEOUT);
    i2c2_stop();

    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);
    return status;
}
