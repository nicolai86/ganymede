#include "rev3.h"
#include "../i2c_master.h"
#include "../oled.h"
#include "hal.h"
#include "../is31fl3733.h"
#include "../m24m01.h"
#include "raw_hid.h"

const is31_led left_hand_matrix[4][6] = {
    {{F_2, D_2, E_2}, {F_5, D_5, E_5}, {F_7, D_7, E_7},    {F_9, D_9, E_9},     {F_12, D_12, E_12},  {F_15, D_15, E_15}},
    {{I_2, G_2, H_2}, {I_5, G_5, H_5}, {I_7, G_7, H_7},    {I_9, G_9, H_9},     {I_12, G_12, H_12},  {I_15, G_15, H_15}},
    {{L_2, J_2, K_2}, {L_5, J_5, K_5}, {L_7, J_7, K_7},    {L_9, J_9, K_9},     {L_12, J_12, K_12},  {L_15, J_15, K_15}},
    {{B_2, A_2, C_2}, {B_5, A_5, C_5}, {C_15, A_15, B_15}, {C_14, A_14, B_14},  {C_13, A_13, B_13},  {C_16, A_16, B_16}},
};

const is31_led left_backlight[9] = {
    {I_11, G_11, H_11},
    {I_6,  G_6,  H_6},
    {I_14, G_14, H_14},
    {I_8,  G_8,  H_8},
    {I_4,  G_4,  H_4},
    {I_13, G_13, H_13},
    {I_10, G_10, H_10},
    {I_3,  G_3,  H_3},
    {I_1,  G_1,  H_1}
};

is31_state left_hand = {
    address: ISSI_ADDR_1,
    rowCount: 4,
    colCount: 6,
    matrix: &left_hand_matrix[0][0],
    backlightCount: 9,
    backlight: &left_backlight[0]
};

const is31_led right_hand_matrix[4][6] = {
    {{F_1, D_1, E_1},    {F_4, D_4, E_4},    {F_6, D_6, E_6},    {F_9, D_9, E_9},    {F_11, D_11, E_11}, {F_14, D_14, E_14}},
    {{I_1, G_1, H_1},    {I_4, G_4, H_4},    {I_6, G_6, H_6},    {I_9, G_9, H_9},    {I_11, G_11, H_11}, {I_14, G_14, H_14}},
    {{L_1, J_1, K_1},    {L_4, J_4, K_4},    {L_6, J_6, K_6},    {L_9, J_9, K_9},    {L_11, J_11, K_11}, {L_14, J_14, K_14}},
    {{B_13, A_13, C_13}, {A_13, B_13, C_13}, {C_13, A_13, B_13}, {C_14, A_14, B_14}, {C_15, A_15, B_15}, {C_16, A_16, B_16}},
};

const is31_led right_backlight[9] = {
    {I_8,  G_8,  H_8},
    {I_5,  G_5,  H_5},
    {I_15, G_15, H_15},
    {I_10, G_10, H_10},
    {I_3,  G_3,  H_3},
    {I_13, G_13, H_13},
    {I_7,  G_7,  H_7},
    {I_2,  G_2,  H_2},
    {I_12, G_12, H_12}
};

is31_state right_hand = {
    address: ISSI_ADDR_2,
    rowCount: 4,
    colCount: 6,
    matrix: &right_hand_matrix[0][0],
    backlightCount: 9,
    backlight: &right_backlight[0]
};

IS31FL3733_ABM ABM1 = {
    T1: IS31FL3733_ABM_T1_840MS,
    T2: IS31FL3733_ABM_T2_1680MS,
    T3: IS31FL3733_ABM_T3_840MS,
    T4: IS31FL3733_ABM_T4_420MS,
    Tbegin:IS31FL3733_ABM_LOOP_BEGIN_T1,
    Tend:IS31FL3733_ABM_LOOP_END_T1,
    Times: IS31FL3733_ABM_LOOP_FOREVER
};

IS31FL3733_ABM ABM2 = {
    T1: IS31FL3733_ABM_T1_840MS,
    T2: IS31FL3733_ABM_T2_1680MS,
    T3: IS31FL3733_ABM_T3_420MS,
    T4: IS31FL3733_ABM_T4_210MS,
    Tbegin:IS31FL3733_ABM_LOOP_BEGIN_T1,
    Tend:  IS31FL3733_ABM_LOOP_END_T1,
    Times: 1
};

static virtual_timer_t led_vt;

is31_animated_led left_hand_colors[4][6] = {{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}},{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}},{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}},{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}}};
is31_animated_led right_hand_colors[4][6] = {{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}},{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}},{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}},{{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30},{0,0,0,1,4,6,5,20,30}}};

bool animate = true;
#define max_brightness 10

int next_value(uint8_t *current, int8_t *increment, uint8_t *limit) {
    int value = *current + *increment;
    if (value >= *limit) {
        *increment = -1;
        value = *limit;
    } else if (value <= 0) {
        value = 0;
        *increment = 1;
        *limit = max_brightness;
    }
    *current = (uint8_t)value;
    return value;
}

static void led_cb(void *arg) {
    if (!animate) { return; }
      for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 6; col++) {
            {
                int r = next_value(&left_hand_colors[row][col].r, &left_hand_colors[row][col].incrementR, &left_hand_colors[row][col].limitR) ;
                int g = next_value(&left_hand_colors[row][col].g, &left_hand_colors[row][col].incrementG, &left_hand_colors[row][col].limitG) ;
                int b = next_value(&left_hand_colors[row][col].b, &left_hand_colors[row][col].incrementB, &left_hand_colors[row][col].limitB) ;
                IS31FL3733_state_set_color(&left_hand, row, col, r, g, b);
            }

            {
                int r = next_value(&right_hand_colors[row][col].r, &right_hand_colors[row][col].incrementR, &right_hand_colors[row][col].limitR) ;
                int g = next_value(&right_hand_colors[row][col].g, &right_hand_colors[row][col].incrementG, &right_hand_colors[row][col].limitG) ;
                int b = next_value(&right_hand_colors[row][col].b, &right_hand_colors[row][col].incrementB, &right_hand_colors[row][col].limitB) ;
                IS31FL3733_state_set_color(&right_hand, row, col, r, g, b);
            }
        }
    }

  chSysLockFromISR();
  chVTSetI(&led_vt, TIME_MS2I(140), led_cb, NULL);
  chSysUnlockFromISR();
}

uint8_t dynamic_keymaps[512];

void matrix_init_user(void)
{
    // debug_enable = true;
    // debug_matrix=true;
    // debug_keyboard=true;

    i2c_init();

    oled_init(OLED_ROTATION_270);
    oled_on();

    init_m24m01();

    // palSetPadMode(GPIOC, 14U, PAL_MODE_INPUT);
    // palPadEnableEventI(GPIOC, 14U, PAL_EVENT_MODE_FALLING_EDGE, leftSideISSIABMInterrupt);
    uint8_t result;
    result = IS31FL3733_init(left_hand.address, 0xff, IS31FL3733_INTERRUPT_IAB, 0xff);
    if (!result) {
        for (int i = 0; i < 192; i ++) {
            IS31FL3733_state_configure_led_abm(&left_hand, i, IS31FL3733_LED_MODE_PWM);
        }
        for ( int index = 0; index < 9; index++ ) {
            IS31FL3733_state_set_led_control_register( &left_hand, &left_backlight[index], true, true, true );
        }
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 6; col++) {
                IS31FL3733_state_set_led_control_register( &left_hand, &left_hand_matrix[row][col], true, true, true );
                // IS31FL3733_state_configure_led_abm(&left_hand, left_hand_matrix[row][col].b, IS31FL3733_LED_MODE_ABM2);
                // IS31FL3733_state_set_color(&left_hand, row, col, 255, 0, 0);
            }
        }

        for (int i = 0; i < 9; i++) {
            IS31FL3733_state_set_backlight_color(&left_hand, i, 0, 0, 175);
        }
        // for (int row = 0; row < 4; row++) {
        //     for (int col = 0; col < 6; col++) {
        //         IS31FL3733_state_set_color(&left_hand, row, col, 255, 0, 0);
        //     }
        // }

        IS31FL3733_state_update_led_control_registers( &left_hand );
        IS31FL3733_state_update_pwm_buffers( &left_hand );

        for (int i = 0; i < 9; i++) {
            IS31FL3733_state_configure_led_abm(&left_hand, left_hand.backlight[i].b, IS31FL3733_LED_MODE_ABM1);
        }

        // for (int row = 0; row < 4; row++) {
        //     for (int col = 0; col < 6; col++) {
        //         IS31FL3733_state_configure_led_abm(&left_hand, left_hand_matrix[row][col].r, IS31FL3733_LED_MODE_ABM1);
        //     }
        // }

        IS31FL3733_configure_abm (&left_hand, IS31FL3733_ABM_NUM_1, &ABM1);
        IS31FL3733_configure_abm (&left_hand, IS31FL3733_ABM_NUM_2, &ABM2);
        IS31FL3733_start_abm (&left_hand);
    } else {
        xprintf("failed to init 0x50: %d\n", result);
    }

    result = IS31FL3733_init(right_hand.address, 0xff, IS31FL3733_INTERRUPT_IAB, 0xff);
    if (!result) {
        for (int i = 0; i < 192; i ++) {
            IS31FL3733_state_configure_led_abm(&right_hand, i, IS31FL3733_LED_MODE_PWM);
        }
        for ( int index = 0; index < 9; index++ ) {
            IS31FL3733_state_set_led_control_register( &right_hand, &right_backlight[index], true, true, true );
        }
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 6; col++) {
                IS31FL3733_state_set_led_control_register( &right_hand, &right_hand_matrix[row][col], true, true, true );
                // IS31FL3733_state_set_color(&right_hand, row, col, 255, 0, 0);
                // IS31FL3733_state_configure_led_abm(&right_hand, right_hand_matrix[row][col].b, IS31FL3733_LED_MODE_ABM2);
            }
        }

        for (int i = 0; i < 9; i++) {
            IS31FL3733_state_set_backlight_color(&right_hand, i, 0, 0, 175);
        }

        // for (int row = 0; row < 4; row++) {
        //     for (int col = 0; col < 6; col++) {
        //         IS31FL3733_state_set_color(&right_hand, row, col, 255, 0, 0);
        //     }
        // }

        IS31FL3733_state_update_led_control_registers( &right_hand );
        IS31FL3733_state_update_pwm_buffers(&right_hand);

        for (int i = 0; i < 9; i++) {
            IS31FL3733_state_configure_led_abm(&right_hand, right_hand.backlight[i].b, IS31FL3733_LED_MODE_ABM1);
        }

        // for (int row = 0; row < 4; row++) {
        //     for (int col = 0; col < 6; col++) {
        //         IS31FL3733_state_configure_led_abm(&right_hand, right_hand_matrix[row][col].r, IS31FL3733_LED_MODE_ABM1);
        //     }
        // }

        IS31FL3733_configure_abm (&right_hand, IS31FL3733_ABM_NUM_1, &ABM1);
        IS31FL3733_configure_abm (&right_hand, IS31FL3733_ABM_NUM_2, &ABM2);
        IS31FL3733_start_abm(&right_hand);
    } else {
        xprintf("failed to init 0x53: %d\n", result);
    }

    chVTObjectInit(&led_vt);
    result = init_m24m01();
    if (result == 0) {
        xprintf("M24M01 ready\n");
    } else {
        xprintf("M24M01 not ready: %d\n", result);
    }

  chVTSet(&led_vt, TIME_MS2I(50), led_cb, NULL);
}

static bool readLeftIssi = false;
static bool readRightIssi = false;

void matrix_scan_user(void)
{
    uint8_t val;
    if (readLeftIssi) {
        readLeftIssi = false;
        IS31FL3733_state_read_register(&left_hand, IS31FL3733_ISR, &val);
        xprintf("left interrupt: %d\n", val);
    }
    if (readRightIssi) {
        readRightIssi = false;
        IS31FL3733_state_read_register(&right_hand, IS31FL3733_ISR, &val);
        xprintf("right interrupt: %d\n", val);

        for (int i = 0; i < 9; i++) {
            IS31FL3733_state_configure_led_abm(&right_hand, right_hand.backlight[i].b, IS31FL3733_LED_MODE_ABM1);
        }
        IS31FL3733_start_abm(&right_hand);
    }

    IS31FL3733_state_update_pwm_buffers(&left_hand);
    IS31FL3733_state_update_pwm_buffers(&right_hand);
}

// static void issi_left_interrupt(EXTDriver *extp, expchannel_t channel) {
//   chSysLockFromISR();
//   extChannelDisableI(&EXTD1, PAL_PAD(C14));

//   xprintf("left issi interrupt\n");
//   readLeftIssi = true;

//   extChannelEnableI(&EXTD1, PAL_PAD(C14));
//   chSysUnlockFromISR();
// }

// static void issi_right_interrupt(EXTDriver *extp, expchannel_t channel) {
//   chSysLockFromISR();
//   extChannelDisableI(&EXTD1, PAL_PAD(C15));

//   xprintf("right issi interrupt\n");
//   readRightIssi = true;

//   extChannelEnableI(&EXTD1, PAL_PAD(C15));
//   chSysUnlockFromISR();
// }

void keyboard_post_init_user(void)
{
    palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 13, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 15, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOB, 12);
    palClearPad(GPIOB, 13);
    palClearPad(GPIOB, 14);
    palClearPad(GPIOB, 15);

    xprintf("keyboard post init columns\n");

    palSetPadMode(GPIOC, 14, PAL_MODE_INPUT);
    palSetPadMode(GPIOC, 15, PAL_MODE_INPUT);

    // static EXTConfig extcfg = {0};
    // static EXTChannelConfig ext_clock_channel_config_left = {
    //     EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, issi_left_interrupt };
    // static EXTChannelConfig ext_clock_channel_config_right = {
    //     EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, issi_right_interrupt };
    // extStart(&EXTD1, &extcfg);
    // extSetChannelModeI(&EXTD1, PAL_PAD(C14), &ext_clock_channel_config_left);
    // extSetChannelModeI(&EXTD1, PAL_PAD(C15), &ext_clock_channel_config_right);
}

const uint8_t IS_EEPROM_READY_COMMAND = 0x80;
const uint8_t READ_EEPROM_BYTE_COMMAND = 0x81;
const uint8_t READ_EEPROM_PAGE_COMMAND = 0x82;

void raw_hid_receive( uint8_t *data, uint8_t length ) {
    uint8_t command = data[0];
    if (command == 0) {
        return;
    }

    xprintf("received raw hid data\n");
    for (int i = 0; i < length; i++) {
        xprintf("%2X", data[i]);
        if (i<length-1) xprintf(", ");
    }
    xprintf("\n");

    wait_ms(100);
    if (length < 2) {
        raw_hid_send(data, length);
        return;
    }

    switch (command) {
        case IS_EEPROM_READY_COMMAND: {
            uint8_t result = init_m24m01();
            data[1] = result;
            raw_hid_send(data, length);
            xprintf("checking if eeprom is ready; %d\n", result);
            break;
        };
        case READ_EEPROM_BYTE_COMMAND: {
            if (length < 3) {
                raw_hid_send(data, length);
                return;
            }
            uint8_t page = data[1];
            uint8_t addr = data[2];
            uint8_t result = 0xFF;
            m24m01_random_byte_read(EEPROM_ADDRESS, (page << 8) | addr, &result);
            data[3] = result;
            raw_hid_send(data, length);
            xprintf("read eeprom at %d/%d: %d\n", page, addr, result);
            break;
        }
        case READ_EEPROM_PAGE_COMMAND: {
            if (length < 2) {
                raw_hid_send(data, length);
                return;
            }
            uint8_t page = data[1];
            uint8_t result[168];
            result[0] = data[0];
            result[1] = data[1];
            m24m01_page_read(EEPROM_ADDRESS, page << 8, &result[2], 128);

            raw_hid_send(result, 130);
            xprintf("read eeprom at %d/0\n", page);
            for (int i = 0; i < 128; i++) {
                xprintf("%2X", result[2+i]);
                if (i < 127) xprintf(", ");
            }
            xprintf("\n");
            break;
        }
    };
}

