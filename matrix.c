/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "i2c_master.h"
#include "pca.h"
#include "oled.h"
#include QMK_KEYBOARD_H

#define print_matrix_header()  print("\nr/c 01234567\n")
#define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#define matrix_bitpop(i)       bitpop(matrix[i])
#define ROW_SHIFTER ((uint8_t)1)

/* matrix state(1:on, 0:off) */
static matrix_row_t raw_matrix[MATRIX_ROWS]; //raw values
static matrix_row_t matrix[MATRIX_ROWS]; //debounced values
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
static void init_pins(void);
static void unselect_rows(void);

__attribute__ ((weak)) void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void)
{
    matrix_scan_user();
}

__attribute__ ((weak))
void matrix_scan_user(void)
{
}

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
  init_pca9675();
  init_pins();

  // initialize matrix state: all keys off
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
      raw_matrix[row] = 0;
      matrix[row] = 0;
  }

  debounce_init(MATRIX_ROWS);

  matrix_init_quantum();
}

static void select_row(uint8_t row) {
  if (row < MATRIX_ROWS_PER_SIDE) {
    setPinOutput(row_pins[row]);
    writePinLow(row_pins[row]);
    return;
  }
  uint8_t ports[] = { 0b11111111, ~(1<<(row-MATRIX_ROWS_PER_SIDE)) };
  i2c_transmit(PCA9675_I2C_ADDR<<1, (void *)&ports, sizeof(ports), PCA9675_I2C_TIMEOUT);
  i2c_stop();
}

static void unselect_row(uint8_t row)
{
  if (row < MATRIX_ROWS_PER_SIDE) {
    setPinInputHigh(row_pins[row]);
    return;
  }
uint8_t ports[] = { 0b11111111, row-MATRIX_ROWS_PER_SIDE,  };
i2c_transmit(PCA9675_I2C_ADDR<<1, (void *)&ports, sizeof(ports), PCA9675_I2C_TIMEOUT);
i2c_stop();
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    matrix_row_t last_row_value = current_matrix[current_row];
    current_matrix[current_row] = 0;

    select_row(current_row);
    wait_us(30);

    bool isLeftSide = current_row < MATRIX_ROWS_PER_SIDE;
    for(uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
      uint8_t pin_state = 1;
      if (isLeftSide) {
        pin_state = readPin(col_pins[col_index]);
      } else {
        uint8_t ports[] = { 0b11111111, 0 };
        i2c_receive((PCA9675_I2C_ADDR<<1)|1, (void *)&ports, sizeof(ports), PCA9675_I2C_TIMEOUT);
        i2c_stop();
        pin_state = (ports[0] & (1 << col_index));
      }

        current_matrix[current_row] |=  pin_state ? 0 : (ROW_SHIFTER << col_index);
    }

    // Unselect col
    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

uint8_t matrix_scan(void)
{
    // TODO i2c error handling
    bool changed = false;

    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
      changed |= read_cols_on_row(raw_matrix, current_row);
    }

    debounce(raw_matrix, matrix, MATRIX_ROWS, changed);
    matrix_scan_quantum();

    oled_task();

    return 1;
}

inline bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1 << col));
}

inline matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void) {
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row);
        print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

uint8_t matrix_key_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += matrix_bitpop(i);
    }
    return count;
}

static void unselect_rows(void)
{
  printf("unselecting all columns\n");
  // pull io expander pins high
  uint8_t ports[] = { 0b11111111, 0b11111111 };
  i2c_transmit(PCA9675_I2C_ADDR<<1, (void *)&ports, sizeof(ports), PCA9675_I2C_TIMEOUT);
  i2c_stop();

  // GPIO
  for(uint8_t x = 0; x < MATRIX_ROWS; x++) {
    if (x < MATRIX_ROWS_PER_SIDE) {
      setPinInput(row_pins[x]);
    }
  }
}

static void init_pins(void) {
  printf("initializing all pins\n");
  unselect_rows();
  for (uint8_t x = 0; x < MATRIX_COLS; x++) {
    setPinInputHigh(col_pins[x]);
  }

  // pull io expander column pins high
  uint8_t ports[] = { 0b11111111, 0b11111111 };
  i2c_transmit(PCA9675_I2C_ADDR<<1, (void *)&ports, sizeof(ports), PCA9675_I2C_TIMEOUT);
  i2c_stop();
}
