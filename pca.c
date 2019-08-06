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
#include "hal.h"
#include "i2c_master.h"
#include "pca.h"

bool i2c_initialized = 0;

uint8_t init_pca9675(void)
{
    if (i2c_initialized == 0) {
        i2c_init();
        i2c_initialized = true;
        chThdSleepMilliseconds(1000);
    }

    uint8_t reset_payload[] = { PCA9675_I2C_RESET_CMD };
    i2c_status_t result;
    result = i2c_transmit(PCA9675_I2C_RESET_ADDR << 1, (void *)&reset_payload, sizeof(reset_payload), PCA9675_I2C_TIMEOUT);
    i2c_stop();

    // P0x as input, P1x as output
    uint8_t port_init[] = { 0b11111111, 0b00000000 };
    result = i2c_transmit(PCA9675_I2C_ADDR << 1, (void *)&port_init, sizeof(port_init), PCA9675_I2C_TIMEOUT);
    i2c_stop();

    // TODO error handling
    // to make the error very visible we'll stop booting at this point

    if (result != I2C_STATUS_SUCCESS) {
        palSetPad(GPIOB, 12);
        palSetPad(GPIOB, 13);
        palSetPad(GPIOB, 14);
        palSetPad(GPIOB, 15);
    }

    return 0;
}
