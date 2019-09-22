
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
#define M24M01_WC_PAD GPIOC
#define M24M01_WC_PIN 13

#define EEPROM_ADDRESS         0xA0
#define EEPROM_LONG_TIMEOUT    1000

uint8_t init_m24m01(void);
uint8_t m24m01_page_read(uint8_t address, uint8_t page, uint8_t*data, uint16_t length);
uint8_t m24m01_page_write(uint8_t address, uint8_t page, const uint8_t*data);
uint8_t m24m01_random_byte_read(uint8_t address, uint8_t addr1, uint8_t addr2, uint8_t *data);
uint8_t m24m01_byte_write(uint8_t address, uint8_t addr1, uint8_t addr2, uint8_t data);
