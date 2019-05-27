#pragma once

#include "config_common.h"

#define STM32_HSECLK    16000000U

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x3061
#define DEVICE_VER      0x0003
#define MANUFACTURER    nicolai86
#define PRODUCT         kepler
#define DESCRIPTION     An split ergonomic keyboard based on the crkbd

#define STM32_BOOTLOADER_ADDRESS 0x1FFFD800

#define MATRIX_ROWS 8
#define MATRIX_ROWS_PER_SIDE 4
#define MATRIX_COLS 6

#define MATRIX_ROW_PINS {B11, B10, B2, B1}
#define MATRIX_COL_PINS {A14, A15, B3, B4, B5, B6 }
#define UNUSED_PINS {}
#define DIODE_DIRECTION COL2ROW

// PCA9675
#define PCA9675_I2C_ADDR       0x20
#define PCA9675_I2C_RESET_ADDR 0x00
#define PCA9675_I2C_RESET_CMD  0x06
#define PCA9675_I2C_TIMEOUT 100

#define I2C1_BANK  GPIOB
#define I2C_DRIVER I2CD1
#define I2C1_SDA 7
#define I2C1_SCL 8


