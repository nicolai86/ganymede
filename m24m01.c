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
#include "m24m01.h"

uint8_t init_m24m01(void) {
    palSetPadMode(M24M01_WC_PAD, M24M01_WC_PIN, PAL_MODE_OUTPUT_PUSHPULL);
    // palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);
    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);

    return 0;
}
