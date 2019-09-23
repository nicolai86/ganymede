/*
Copyright 2019 Ryan Caltabiano <https://github.com/XScorpion2>

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
#ifndef OLEDH
#include "i2c_master.h"

#define OLEDH

#define OLED_DISPLAY_ADDRESS       0x3C

#define OLED_DISPLAY_WIDTH 128
#define OLED_DISPLAY_HEIGHT 32
#define OLED_MATRIX_SIZE (OLED_DISPLAY_HEIGHT / 8 * OLED_DISPLAY_WIDTH) // 512 (compile time mathed)
#define OLED_BLOCK_TYPE uint16_t // Type to use for segmenting the oled display for smart rendering, use unsigned types only
#define OLED_BLOCK_COUNT (sizeof(OLED_BLOCK_TYPE) * 8) // 16 (compile time mathed)
#define OLED_BLOCK_SIZE (OLED_MATRIX_SIZE / OLED_BLOCK_COUNT) // 32 (compile time mathed)

// For 90 degree rotation, we map our internal matrix to oled matrix using fixed arrays
// The OLED writes to it's memory horizontally, starting top left, but our memory starts bottom left in this mode
#define OLED_SOURCE_MAP { 0, 8, 16, 24 }
#define OLED_TARGET_MAP { 24, 16, 8, 0 }

#define OLED_FONT_START 0
#define OLED_FONT_END 224

#define OLED_FONT_WIDTH 6
#define OLED_FONT_HEIGHT 8

#define MatrixRows (OLED_DISPLAY_HEIGHT / OLED_FONT_HEIGHT)
#define MatrixCols (OLED_DISPLAY_WIDTH / OLED_FONT_WIDTH)

#include <stdint.h>
#include <stdbool.h>

// OLED Rotation enum values are flags
typedef enum {
    OLED_ROTATION_0   = 0,
    OLED_ROTATION_90  = 1,
    OLED_ROTATION_180 = 2,
    OLED_ROTATION_270 = 3, // OLED_ROTATION_90 | OLED_ROTATION_180
} oled_rotation_t;

void oled_i2c_init(void);

// Initialize the oled display, rotating the rendered output based on the define passed in.
// Returns true if the OLED was initialized successfully
bool oled_init(oled_rotation_t rotation);

// Called at the start of oled_init, weak function overridable by the user
// rotation - the value passed into oled_init
// Return new oled_rotation_t if you want to override default rotation
oled_rotation_t oled_init_user(oled_rotation_t rotation);

// Clears the display buffer, resets cursor position to 0, and sets the buffer to dirty for rendering
void oled_clear(void);

// Renders the dirty chunks of the buffer to oled display
void oled_render(void);

// Moves cursor to character position indicated by column and line, wraps if out of bounds
// Max column denoted by 'oled_max_chars()' and max lines by 'oled_max_lines()' functions
void oled_set_cursor(uint8_t col, uint8_t line);

// Advances the cursor to the next page, writing ' ' if true
// Wraps to the begining when out of bounds
void oled_advance_page(bool clearPageRemainder);

// Moves the cursor forward 1 character length
// Advance page if there is not enough room for the next character
// Wraps to the begining when out of bounds
void oled_advance_char(void);

// Writes a single character to the buffer at current cursor position
// Advances the cursor while writing, inverts the pixels if true
// Main handler that writes character data to the display buffer
void oled_write_char(const char data, bool invert);

// Writes a string to the buffer at current cursor position
// Advances the cursor while writing, inverts the pixels if true
void oled_write(const char *data, bool invert);

// Writes a string to the buffer at current cursor position
// Advances the cursor while writing, inverts the pixels if true
// Advances the cursor to the next page, wiring ' ' to the remainder of the current page
void oled_write_ln(const char *data, bool invert);

// Writes a string to the buffer at current cursor position
// Advances the cursor while writing, inverts the pixels if true
#define oled_write_P(data, invert) oled_write(data, invert)

// Writes a string to the buffer at current cursor position
// Advances the cursor while writing, inverts the pixels if true
// Advances the cursor to the next page, wiring ' ' to the remainder of the current page
#define oled_write_ln_P(data, invert) oled_write(data, invert)

// Can be used to manually turn on the screen if it is off
// Returns true if the screen was on or turns on
bool oled_on(void);

// Can be used to manually turn off the screen if it is on
// Returns true if the screen was off or turns off
bool oled_off(void);

// Basically it's oled_render, but with timeout management and oled_task_user calling!
void oled_task(void);

// Called at the start of oled_task, weak function overridable by the user
void oled_task_user(void);

// Scrolls the entire display right
// Returns true if the screen was scrolling or starts scrolling
// NOTE: display contents cannot be changed while scrolling
bool oled_scroll_right(void);

// Scrolls the entire display left
// Returns true if the screen was scrolling or starts scrolling
// NOTE: display contents cannot be changed while scrolling
bool oled_scroll_left(void);

// Turns off display scrolling
// Returns true if the screen was not scrolling or stops scrolling
bool oled_scroll_off(void);

// Returns the maximum number of characters that will fit on a line
uint8_t oled_max_chars(void);

// Returns the maximum number of lines that will fit on the oled
uint8_t oled_max_lines(void);


//
i2c_status_t i2c2_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c2_transmit(uint8_t address, const uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c2_transceive(uint8_t address, uint8_t *writeData, uint16_t writeLength, uint8_t *readData, uint16_t readLength, uint16_t timeout);
i2c_status_t i2c2_getErrors(void);
i2c_status_t i2c2_receive(uint8_t address, uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c2_isDeviceReady(uint8_t address, uint16_t timeout);
void i2c2_stop(void);
void i2c2_init(void);

#endif
