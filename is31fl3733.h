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


#ifndef IS31FL3733_DRIVER_H
#define IS31FL3733_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct is31_led {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} __attribute__((packed)) is31_led;

typedef struct is31_state {
  uint8_t address;
  uint8_t rowCount;
  uint8_t colCount;
  const is31_led *matrix;

  const uint8_t backlightCount;
  const is31_led *backlight;

  uint8_t pwm_buffer[192];
  uint8_t autobreath_buffer[192];
  uint8_t led_control_registers[24];
} is31_state;

uint8_t IS31FL3733_init( uint8_t addr, uint8_t sync );

void IS31FL3733_state_update_pwm_buffers(is31_state*state);
void IS31FL3733_state_update_led_control_registers( is31_state *state );
void IS31FL3733_state_set_color(is31_state * state, uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue );
void IS31FL3733_state_set_backlight_color(is31_state * state, uint8_t index, uint8_t red, uint8_t green, uint8_t blue );
void IS31FL3733_state_set_led_control_register( is31_state *state, const is31_led*led, bool red, bool green, bool blue );

#define A_1  0x00
#define A_2  0x01
#define A_3  0x02
#define A_4  0x03
#define A_5  0x04
#define A_6  0x05
#define A_7  0x06
#define A_8  0x07
#define A_9  0x08
#define A_10  0x09
#define A_11  0x0A
#define A_12  0x0B
#define A_13  0x0C
#define A_14  0x0D
#define A_15  0x0E
#define A_16  0x0F

#define B_1  0x10
#define B_2  0x11
#define B_3  0x12
#define B_4  0x13
#define B_5  0x14
#define B_6  0x15
#define B_7  0x16
#define B_8  0x17
#define B_9  0x18
#define B_10  0x19
#define B_11  0x1A
#define B_12  0x1B
#define B_13  0x1C
#define B_14  0x1D
#define B_15  0x1E
#define B_16  0x1F

#define C_1  0x20
#define C_2  0x21
#define C_3  0x22
#define C_4  0x23
#define C_5  0x24
#define C_6  0x25
#define C_7  0x26
#define C_8  0x27
#define C_9  0x28
#define C_10  0x29
#define C_11  0x2A
#define C_12  0x2B
#define C_13  0x2C
#define C_14  0x2D
#define C_15  0x2E
#define C_16  0x2F

#define D_1  0x30
#define D_2  0x31
#define D_3  0x32
#define D_4  0x33
#define D_5  0x34
#define D_6  0x35
#define D_7  0x36
#define D_8  0x37
#define D_9  0x38
#define D_10  0x39
#define D_11  0x3A
#define D_12  0x3B
#define D_13  0x3C
#define D_14  0x3D
#define D_15  0x3E
#define D_16  0x3F

#define E_1  0x40
#define E_2  0x41
#define E_3  0x42
#define E_4  0x43
#define E_5  0x44
#define E_6  0x45
#define E_7  0x46
#define E_8  0x47
#define E_9  0x48
#define E_10  0x49
#define E_11  0x4A
#define E_12  0x4B
#define E_13  0x4C
#define E_14  0x4D
#define E_15  0x4E
#define E_16  0x4F

#define F_1  0x50
#define F_2  0x51
#define F_3  0x52
#define F_4  0x53
#define F_5  0x54
#define F_6  0x55
#define F_7  0x56
#define F_8  0x57
#define F_9  0x58
#define F_10  0x59
#define F_11  0x5A
#define F_12  0x5B
#define F_13  0x5C
#define F_14  0x5D
#define F_15  0x5E
#define F_16  0x5F

#define G_1  0x60
#define G_2  0x61
#define G_3  0x62
#define G_4  0x63
#define G_5  0x64
#define G_6  0x65
#define G_7  0x66
#define G_8  0x67
#define G_9  0x68
#define G_10  0x69
#define G_11  0x6A
#define G_12  0x6B
#define G_13  0x6C
#define G_14  0x6D
#define G_15  0x6E
#define G_16  0x6F

#define H_1  0x70
#define H_2  0x71
#define H_3  0x72
#define H_4  0x73
#define H_5  0x74
#define H_6  0x75
#define H_7  0x76
#define H_8  0x77
#define H_9  0x78
#define H_10  0x79
#define H_11  0x7A
#define H_12  0x7B
#define H_13  0x7C
#define H_14  0x7D
#define H_15  0x7E
#define H_16  0x7F

#define I_1  0x80
#define I_2  0x81
#define I_3  0x82
#define I_4  0x83
#define I_5  0x84
#define I_6  0x85
#define I_7  0x86
#define I_8  0x87
#define I_9  0x88
#define I_10  0x89
#define I_11  0x8A
#define I_12  0x8B
#define I_13  0x8C
#define I_14  0x8D
#define I_15  0x8E
#define I_16  0x8F

#define J_1  0x90
#define J_2  0x91
#define J_3  0x92
#define J_4  0x93
#define J_5  0x94
#define J_6  0x95
#define J_7  0x96
#define J_8  0x97
#define J_9  0x98
#define J_10  0x99
#define J_11  0x9A
#define J_12  0x9B
#define J_13  0x9C
#define J_14  0x9D
#define J_15  0x9E
#define J_16  0x9F

#define K_1  0xA0
#define K_2  0xA1
#define K_3  0xA2
#define K_4  0xA3
#define K_5  0xA4
#define K_6  0xA5
#define K_7  0xA6
#define K_8  0xA7
#define K_9  0xA8
#define K_10  0xA9
#define K_11  0xAA
#define K_12  0xAB
#define K_13  0xAC
#define K_14  0xAD
#define K_15  0xAE
#define K_16  0xAF

#define L_1  0xB0
#define L_2  0xB1
#define L_3  0xB2
#define L_4  0xB3
#define L_5  0xB4
#define L_6  0xB5
#define L_7  0xB6
#define L_8  0xB7
#define L_9  0xB8
#define L_10  0xB9
#define L_11  0xBA
#define L_12  0xBB
#define L_13  0xBC
#define L_14  0xBD
#define L_15  0xBE
#define L_16  0xBF

// taken from https://github.com/kkostyan/is31fl3733
// adjusted to work with the modified QMK code
#define IS31FL3733_ABM_LOOP_FOREVER (0x0000)
#define IS31FL3733_LEDABM (0x02) /// Auto breath mode for each LED. Write only.
#define IS31FL3733_ABM1  (0x02) /// Auto breath control register for ABM-1. Write only.
#define IS31FL3733_ABM2  (0x06) /// Auto breath control register for ABM-2. Write only.
#define IS31FL3733_ABM3  (0x0A) /// Auto breath control register for ABM-3. Write only.
#define IS31FL3733_TUR   (0x0E) /// Time update register. Write only.
#define IS31FL3733_SWPUR (0x0F) /// SWy Pull-Up Resistor selection register. Write only.
#define IS31FL3733_CSPDR (0x10) /// CSx Pull-Down Resistor selection register. Write only.
#define IS31FL3733_RESET (0x11) /// Reset register. Read only.
/// CR register bits.
#define IS31FL3733_CR_SYNC_MASTER (0x40) /// Configure as clock master device.
#define IS31FL3733_CR_SYNC_SLAVE  (0x80) /// Configure as clock slave device.
#define IS31FL3733_CR_OSD         (0x04) /// Open/Short detection enable bit.
#define IS31FL3733_CR_BEN         (0x02) /// Auto breath mode enable bit.
#define IS31FL3733_CR_SSD         (0x01) /// Software shutdown bit.

/// LED mode enumeration.
typedef enum {
  IS31FL3733_LED_MODE_PWM  = 0x00, ///< PWM control mode.
  IS31FL3733_LED_MODE_ABM1 = 0x01, ///< Auto Breath Mode 1.
  IS31FL3733_LED_MODE_ABM2 = 0x02, ///< Auto Breath Mode 2.
  IS31FL3733_LED_MODE_ABM3 = 0x03  ///< Auto Breath Mode 3.
} IS31FL3733_LED_MODE;

/// ABM T1 period time, ms.
typedef enum {
  IS31FL3733_ABM_T1_210MS   = 0x00,
  IS31FL3733_ABM_T1_420MS   = 0x20,
  IS31FL3733_ABM_T1_840MS   = 0x40,
  IS31FL3733_ABM_T1_1680MS  = 0x60,
  IS31FL3733_ABM_T1_3360MS  = 0x80,
  IS31FL3733_ABM_T1_6720MS  = 0xA0,
  IS31FL3733_ABM_T1_13440MS = 0xC0,
  IS31FL3733_ABM_T1_26880MS = 0xE0
} IS31FL3733_ABM_T1;

/// ABM T2 period time, ms.
typedef enum {
  IS31FL3733_ABM_T2_0MS     = 0x00,
  IS31FL3733_ABM_T2_210MS   = 0x02,
  IS31FL3733_ABM_T2_420MS   = 0x04,
  IS31FL3733_ABM_T2_840MS   = 0x06,
  IS31FL3733_ABM_T2_1680MS  = 0x08,
  IS31FL3733_ABM_T2_3360MS  = 0x0A,
  IS31FL3733_ABM_T2_6720MS  = 0x0C,
  IS31FL3733_ABM_T2_13440MS = 0x0E,
  IS31FL3733_ABM_T2_26880MS = 0x10
} IS31FL3733_ABM_T2;

/// ABM T3 period time, ms.
typedef enum {
  IS31FL3733_ABM_T3_210MS   = 0x00,
  IS31FL3733_ABM_T3_420MS   = 0x20,
  IS31FL3733_ABM_T3_840MS   = 0x40,
  IS31FL3733_ABM_T3_1680MS  = 0x60,
  IS31FL3733_ABM_T3_3360MS  = 0x80,
  IS31FL3733_ABM_T3_6720MS  = 0xA0,
  IS31FL3733_ABM_T3_13440MS = 0xC0,
  IS31FL3733_ABM_T3_26880MS = 0xE0
} IS31FL3733_ABM_T3;

/// ABM T4 period time, ms.
typedef enum {
  IS31FL3733_ABM_T4_0MS      = 0x00,
  IS31FL3733_ABM_T4_210MS    = 0x02,
  IS31FL3733_ABM_T4_420MS    = 0x04,
  IS31FL3733_ABM_T4_840MS    = 0x06,
  IS31FL3733_ABM_T4_1680MS   = 0x08,
  IS31FL3733_ABM_T4_3360MS   = 0x0A,
  IS31FL3733_ABM_T4_6720MS   = 0x0C,
  IS31FL3733_ABM_T4_13440MS  = 0x0E,
  IS31FL3733_ABM_T4_26880MS  = 0x10,
  IS31FL3733_ABM_T4_53760MS  = 0x12,
  IS31FL3733_ABM_T4_107520MS = 0x14
} IS31FL3733_ABM_T4;

/// ABM loop beginning time.
typedef enum {
  IS31FL3733_ABM_LOOP_BEGIN_T1 = 0x00, ///< Loop begin from T1.
  IS31FL3733_ABM_LOOP_BEGIN_T2 = 0x10, ///< Loop begin from T2.
  IS31FL3733_ABM_LOOP_BEGIN_T3 = 0x20, ///< Loop begin from T3.
  IS31FL3733_ABM_LOOP_BEGIN_T4 = 0x30  ///< Loop begin from T4.
} IS31FL3733_ABM_LOOP_BEGIN;

/// ABM loop end time.
typedef enum {
  IS31FL3733_ABM_LOOP_END_T3 = 0x00, ///< Loop end at end of T3.
  IS31FL3733_ABM_LOOP_END_T1 = 0x40  ///< Loop end at end of T1.
} IS31FL3733_ABM_LOOP_END;

/// ABM function number (also used as register offset).
typedef enum {
  IS31FL3733_ABM_NUM_1 = IS31FL3733_ABM1,
  IS31FL3733_ABM_NUM_2 = IS31FL3733_ABM2,
  IS31FL3733_ABM_NUM_3 = IS31FL3733_ABM3
} IS31FL3733_ABM_NUM;

/** Auto Breath Mode (ABM) configuration structure.
  *      +----+              +
  *     /      \            /
  *    /        \          /
  *   /          \        /
  *  /            \      /
  * +              +----+
  * | T1 | T2 | T3 | T4 | T1 |
  *
  */
typedef struct {
  /// T1 time.
  IS31FL3733_ABM_T1 T1;
  /// T2 time.
  IS31FL3733_ABM_T2 T2;
  /// T3 time.
  IS31FL3733_ABM_T3 T3;
  /// T4 time.
  IS31FL3733_ABM_T4 T4;
  /// Loop beginning time.
  IS31FL3733_ABM_LOOP_BEGIN Tbegin;
  /// Loop end time.
  IS31FL3733_ABM_LOOP_END Tend;
  /// Total loop times.
  uint16_t Times;
} IS31FL3733_ABM;

void IS31FL3733_state_configure_led_abm(is31_state *state,  uint8_t led, IS31FL3733_LED_MODE mode);
void IS31FL3733_configure_abm (is31_state *state, IS31FL3733_ABM_NUM n, IS31FL3733_ABM *config);
void IS31FL3733_start_abm(is31_state *state);

#endif // IS31FL3733_DRIVER_H
