/* Copyright 2017 Jason Williams
 * Copyright 2018 Jack Humbert
 * Copyright 2018 Yiancar
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

#include "wait.h"
#include "debug.h"
#include "is31fl3733.h"
#include <string.h>
#include "i2c_master.h"
#include "progmem.h"

// This is a 7-bit address, that gets left-shifted and bit 0
// set to 0 for write, 1 for read (as per I2C protocol)
// The address will vary depending on your wiring:
// 00 <-> GND
// 01 <-> SCL
// 10 <-> SDA
// 11 <-> VCC
// ADDR1 represents A1:A0 of the 7-bit address.
// ADDR2 represents A3:A2 of the 7-bit address.
// The result is: 0b101(ADDR2)(ADDR1)
#define ISSI_ADDR_DEFAULT 0x50

#define ISSI_COMMANDREGISTER 0xFD
#define ISSI_COMMANDREGISTER_WRITELOCK 0xFE
#define ISSI_INTERRUPTMASKREGISTER 0xF0
#define ISSI_INTERRUPTSTATUSREGISTER 0xF1

#define ISSI_PAGE_LEDCONTROL 0x00 //PG0
#define ISSI_PAGE_PWM 0x01        //PG1
#define ISSI_PAGE_AUTOBREATH 0x02 //PG2
#define ISSI_PAGE_FUNCTION 0x03   //PG3

#define ISSI_REG_CONFIGURATION 0x00 //PG3
#define ISSI_REG_GLOBALCURRENT 0x01 //PG3
#define ISSI_REG_RESET 0x11// PG3
#define ISSI_REG_SWPULLUP 0x0F //PG3
#define ISSI_REG_CSPULLUP 0x10 //PG3

#ifndef ISSI_TIMEOUT
  #define ISSI_TIMEOUT 100
#endif

// Transfer buffer for TWITransmitData()
uint8_t g_twi_transfer_buffer[20];

uint8_t IS31FL3733_write_register( uint8_t addr, uint8_t reg, uint8_t data )
{
    g_twi_transfer_buffer[0] = reg;
    g_twi_transfer_buffer[1] = data;

    return i2c_transmit(addr << 1, g_twi_transfer_buffer, 2, ISSI_TIMEOUT);
}

void IS31FL3733_write_buffer( uint8_t addr, uint8_t page, uint8_t *pwm_buffer )
{
    // select PG1
    IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER, page );

    // transmit PWM registers in 12 transfers of 16 bytes
    // g_twi_transfer_buffer[] is 20 bytes

    // iterate over the pwm_buffer contents at 16 byte intervals
    for ( int i = 0; i < 192; i += 16 ) {
        g_twi_transfer_buffer[0] = i;
        // copy the data from i to i+15
        // device will auto-increment register for data after the first byte
        // thus this sets registers 0x00-0x0F, 0x10-0x1F, etc. in one transfer
        for ( int j = 0; j < 16; j++ ) {
            g_twi_transfer_buffer[1 + j] = pwm_buffer[i + j];
        }

        while (i2c_transmit(addr << 1, g_twi_transfer_buffer, 17, ISSI_TIMEOUT)) {};
    }
    i2c_stop();
}

uint8_t IS31FL3733_init( uint8_t addr, uint8_t sync)
{
    // In order to avoid the LEDs being driven with garbage data
    // in the LED driver's PWM registers, shutdown is enabled last.
    // Set up the mode and other settings, clear the PWM registers,
    // then disable software shutdown.
    // Sync is passed so set it according to the datasheet.

    // Unlock the command register.
    i2c_status_t result;
    result = IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
    if (result) {
        for (;;) printf("failed to init: 1: %d\n", result);
        return result;
    }

    // Select PG0
    result = IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER, ISSI_PAGE_LEDCONTROL );
    if (result) {
        for (;;) printf("failed to init: 2\n");
        return result;
    }

    // Turn off all LEDs.
    for ( int i = 0x00; i <= 0x17; i++ )
    {
        result = IS31FL3733_write_register( addr, i, 0x00 );
        if (result) {
            for (;;) printf("failed to init: 3\n");
            return result;
        }
    }

    // Unlock the command register.
    result = IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
    if (result) {
        for (;;) printf("failed to init: 4\n");
        return result;
    }

    // Select PG1
    result = IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER, ISSI_PAGE_PWM );
    if (result) {
        for (;;) printf("failed to init: 5\n");
        return result;
    }
    // Set PWM on all LEDs to 0
    // No need to setup Breath registers to PWM as that is the default.
    for ( int i = 0x00; i <= 0xBF; i++ )
    {
        result = IS31FL3733_write_register( addr, i, 0x00 );
        if (result) {
            for (;;) printf("failed to init: 6\n");
            return result;
        }
    }

    // Unlock the command register.
    result = IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
    if (result) {
        for (;;) printf("failed to init: 7\n");
        return result;
    }

    // Select PG3
    result = IS31FL3733_write_register( addr, ISSI_COMMANDREGISTER, ISSI_PAGE_FUNCTION );
    if (result) {
        for (;;) printf("failed to init: 8\n");
        return result;
    }
    // Set global current to maximum.
    result = IS31FL3733_write_register( addr, ISSI_REG_GLOBALCURRENT, 0xFF );
    if (result) {
        for (;;) printf("failed to init: 9\n");
        return result;
    }
    // Disable software shutdown, enable breath
    result = IS31FL3733_write_register( addr, ISSI_REG_CONFIGURATION, (sync << 6) | 0x01 | 0x02 );
    if (result) {
        for (;;) printf("failed to init: 10\n");
        return result;
    }

    // Wait 10ms to ensure the device has woken up.
    wait_ms(10);
    return result;
}

void IS31FL3733_state_set_color(is31_state * state, uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue ) {
    is31_led led = *(state->matrix + (row * state->colCount + col));
    state->pwm_buffer[led.r] = red;
    state->pwm_buffer[led.g] = green;
    state->pwm_buffer[led.b] = blue;
}

void IS31FL3733_state_set_backlight_color(is31_state * state, uint8_t index, uint8_t red, uint8_t green, uint8_t blue ) {
    is31_led led = *(state->backlight + index);
    state->pwm_buffer[led.r] = red;
    state->pwm_buffer[led.g] = green;
    state->pwm_buffer[led.b] = blue;
}

void IS31FL3733_state_set_led_control_register( is31_state *state, const is31_led*led, bool red, bool green, bool blue ) {
    uint8_t control_register_r = led->r / 8;
    uint8_t control_register_g = led->g / 8;
    uint8_t control_register_b = led->b / 8;
    uint8_t bit_r = led->r % 8;
    uint8_t bit_g = led->g % 8;
    uint8_t bit_b = led->b % 8;

    if ( red ) {
        state->led_control_registers[control_register_r] |= (1 << bit_r);
    } else {
        state->led_control_registers[control_register_r] &= ~(1 << bit_r);
    }
    if ( green ) {
        state->led_control_registers[control_register_g] |= (1 << bit_g);
    } else {
        state->led_control_registers[control_register_g] &= ~(1 << bit_g);
    }
    if ( blue ) {
        state->led_control_registers[control_register_b] |= (1 << bit_b);
    } else {
        state->led_control_registers[control_register_b] &= ~(1 << bit_b);
    }
}

void IS31FL3733_state_update_pwm_buffers(is31_state*state) {
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER, ISSI_PAGE_PWM );

    IS31FL3733_write_buffer( state->address, ISSI_PAGE_PWM, state->pwm_buffer );
}

void IS31FL3733_state_update_led_control_registers( is31_state *state ) {
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER, ISSI_PAGE_LEDCONTROL );
    for ( int i=0; i<24; i++ )
    {
        IS31FL3733_write_register(state->address, i, state->led_control_registers[i] );
    }
}

void IS31FL3733_state_configure_led_abm(is31_state *state, uint8_t led, IS31FL3733_LED_MODE mode) {
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER, ISSI_PAGE_AUTOBREATH );
    IS31FL3733_write_register(state->address, led, mode);
}

void IS31FL3733_configure_abm (is31_state *state, IS31FL3733_ABM_NUM n, IS31FL3733_ABM *config) {
    IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
  IS31FL3733_write_register(state->address, ISSI_COMMANDREGISTER, ISSI_PAGE_FUNCTION );
  // Set fade in and fade out time.
  IS31FL3733_write_register(state->address, n, config->T1 | config->T2);
  // Set hold and off time.
  IS31FL3733_write_register(state->address, n + 1, config->T3 | config->T4);
  // Set loop begin/end time and high part of loop times.
  IS31FL3733_write_register(state->address, n + 2, config->Tend | config->Tbegin | ((config->Times >> 8) & 0x0F));
  // Set low part of loop times.
  IS31FL3733_write_register(state->address, n + 3, config->Times & 0xFF);
 IS31FL3733_write_register(state->address, ISSI_COMMANDREGISTER, ISSI_PAGE_PWM );

}

void IS31FL3733_start_abm(is31_state *state) {
  // Clear B_EN bit in configuration register.
  // Write 0x00 to Time Update Register to update ABM settings.
  IS31FL3733_write_register( state->address, ISSI_COMMANDREGISTER_WRITELOCK, 0xC5 );
  IS31FL3733_write_register(state->address, ISSI_COMMANDREGISTER, ISSI_PAGE_FUNCTION );
  IS31FL3733_write_register(state->address, ISSI_COMMANDREGISTER, IS31FL3733_CR_SSD);
  IS31FL3733_write_register(state->address, ISSI_COMMANDREGISTER, IS31FL3733_CR_BEN | IS31FL3733_CR_SSD);
  IS31FL3733_write_register(state->address, IS31FL3733_TUR, 0x00);
}

