#include "rev1.h"
#include "../i2c_master.h"
#include "../oled.h"
#include "hal.h"

void matrix_init_user(void) {
  debug_enable=true;

  i2c_init();
}

void keyboard_post_init_user(void) {
  oled_init(OLED_ROTATION_270);
  oled_on();

  palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 14, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 15, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(GPIOB, 12);
  palClearPad(GPIOB, 13);
  palClearPad(GPIOB, 14);
  palClearPad(GPIOB, 15);
  palClearPad(GPIOA, 8);

  printf("keyboard post init columns\n");
}
