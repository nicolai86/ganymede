#include "rev3.h"
#include "../i2c_master.h"
#include "../oled.h"
#include "hal.h"
#include "../is31fl3733.h"
#include "../m24m01.h"

const is31_led left_hand_matrix[4][6] = {
    {{F_2, D_2, E_2}, {F_5, D_5, E_5}, {F_7, D_7, E_7},    {F_9, D_9, E_9},     {F_12, D_12, E_12},  {F_15, D_15, E_15}},
    {{I_2, G_2, H_2}, {I_5, G_5, H_5}, {I_7, G_7, H_7},    {I_9, G_9, H_9},     {I_12, G_12, H_12},  {I_15, G_15, H_15}},
    {{L_2, J_2, K_2}, {L_5, J_5, K_5}, {L_7, J_7, K_7},    {L_9, J_9, K_9},     {L_12, J_12, K_12},  {L_15, J_15, K_15}},
    {{B_2, A_2, C_2}, {B_5, A_5, C_5}, {C_15, A_15, B_15}, {C_14, A_14, B_14},  {C_13, A_13, B_13},  {C_16, A_16, B_16}},
};

const is31_led left_backlight[9] = {
    {I_11, G_11 , H_11},
    {I_6, G_6 , H_6},
    {I_14, G_14 , H_14},
    {I_8, G_8 , H_8},
    {I_4, G_4 , H_4},
    {I_13, G_13 , H_13},
    {I_10, G_10 , H_10},
    {I_3, G_3 , H_3},
    {I_1, G_1 , H_1}
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
    {{F_1, E_1, D_1}, {F_4, E_4, D_4}, {F_6, E_6, D_6}, {F_9, E_9, D_9}, {F_11, E_11, D_11}, {F_14, E_14, D_14}},
    {{I_1, H_1, G_1}, {I_4, H_4, G_4}, {I_6, H_6, G_6}, {I_9, H_9, G_9}, {I_11, H_11, G_11}, {I_14, H_14, G_14}},
    {{L_1, K_1, J_1},{L_4, K_4, J_4}, {L_6, K_6, J_6}, {L_9, K_9, J_9}, {L_11, K_11, J_11}, {L_14, K_14, J_14}},
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

void matrix_init_user(void)
{
    // debug_enable = true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;

    i2c_init();

    init_m24m01();

    oled_init(OLED_ROTATION_270);
    oled_on();

    uint8_t result;
    result = IS31FL3733_init(left_hand.address, IS31FL3733_CR_SYNC_MASTER);
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
            IS31FL3733_state_set_backlight_color(&left_hand, i, 0, 0, 255);
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
        printf("failed to init 0x50: %d\n", result);
    }

    result = IS31FL3733_init(right_hand.address, IS31FL3733_CR_SYNC_SLAVE);
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
            IS31FL3733_state_set_backlight_color(&right_hand, i, 0, 0, 255);
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
        printf("failed to init 0x53: %d\n", result);
    }
}

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

    printf("keyboard post init columns\n");
}
