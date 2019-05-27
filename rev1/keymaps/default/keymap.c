#include QMK_KEYBOARD_H
#include "../../../oled.h"

#define _QWERTY 0
#define _LOWER 3
#define _RAISE 4
#define _ADJUST 16

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_LOWER LOWER
#define KC_RAISE RAISE
#define KC_RST   RESET
#define KC_CTLTB CTL_T(KC_TAB)
#define KC_ALTZ  ALT_T(KC_Z)
#define KC_GUI_EQ RGUI_T(KC_EQUAL)
#define KC_GUI_QUO GUI_T(KC_QUOTE)
#define KC_ALTSLSH ALT_T(KC_SLASH)
#define KC_CTRLX CTL_T(KC_X)

enum keyboard_layers {
  QWERTY,
  LOWER,
  RAISE,
  ADJUST,
};

void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  oled_clear();

  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }

  bool didPrint = false;
  if (layer_state & (1<<_LOWER)) {
    palSetPad(GPIOB, 13);
    oled_write_ln_P(PSTR("LOWER"), false);
    didPrint = true;
  } else {
    palClearPad(GPIOB, 13);
  }

  if (layer_state & (1<<_RAISE)) {
   palSetPad(GPIOB, 12);
   oled_write_ln_P(PSTR("RAISE"), false);
   didPrint = true;
  } else {
    palClearPad(GPIOB, 12);
  }

  if (!didPrint) {
    oled_write_ln_P(PSTR("BASE"), false);
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [QWERTY] = LAYOUT_kc(
      ESC,      Q,   W,     E,     R,     T, \
      GUI_EQ,   A,   S,     D,     F,     G, \
      LSHIFT,   Z,   X,     C,     V,     B, \
      LCTL,   LALT,     LOWER,  RAISE, SPC,  BSPC, \

            Y,     U,     I,     O,     P,     BSLASH, \
            H,     J,     K,     L,     SCLN,  GUI_QUO, \
            N,     M,     COMM,  DOT,   SLASH, RSHIFT, \
     TAB,  ENTER,        LOWER,RAISE,    RALT,   RCTL \
    ),
    [_LOWER] = LAYOUT_kc( \
       TRNS,     1,     2,     3,     4,     5,\
       TRNS,    F1,    F2,    F3,    F4,    F5,\
       TRNS, LALT, GRAVE,    APP, XXXXX, XXXXX,\
       TRNS,  TRNS, TRNS,   TRNS,  TRNS, TRNS,\

        6,     7,     8,     9,     0,  TRNS, \
        F6,    F7,    F8,    F9,   F10,  TRNS,\
        XXXXX,  LEFT,    UP,  DOWN, RIGHT,  TRNS,\
     HOME,   END, TRNS,   TRNS,   TRNS, TRNS
  ),
  [_RAISE] = LAYOUT_kc( \
       CIRC,  AMPR,  LPRN,  RPRN,  ASTR, XXXXX, \
       MINS,   EQL,  LCBR,  RCBR,  PIPE,   GRV, \
       UNDS,  PLUS,  LBRC,  RBRC,  BSLS,  TILD, \
        TRNS,  TRNS, TRNS,   TRNS,  TRNS,  TRNS,\

                TRNS,  TRNS, TRNS,  TRNS,  TRNS,  TRNS, \
                TRNS,  TRNS, TRNS,  TRNS,  TRNS,  TRNS, \
                LCTRL, TRNS,TRNS,  TRNS,  TRNS,  TRNS, \
                TRNS,  TRNS, TRNS, TRNS,  TRNS,  RST
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_RST: // Custom RESET code
      if (!record->event.pressed) {
        reset_keyboard();
      }
    return false;
    case QWERTY:
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
  }
  return true;
}
