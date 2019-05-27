#ifndef KEPLERH
#define KEPLERH

#include "quantum.h"

#define LAYOUT_all(\
    k00, k01, k02, k03, k04, k05, \
    k10, k11, k12, k13, k14, k15, \
    k20, k21, k22, k23, k24, k25, \
    k30, k31, k32, k33, k34, k35, \
    k45, k44, k43, k42, k41, k40, \
    k55, k54, k53, k52, k51, k50, \
    k65, k64, k63, k62, k61, k60, \
    k75, k74, k73, k72, k71, k70 \
) { \
    { k00, k01, k02, k03, k04, k05 }, \
    { k10, k11, k12, k13, k14, k15 }, \
    { k20, k21, k22, k23, k24, k25 }, \
    { k30, k31, k32, k33, k34, k35 }, \
    { k40, k41, k42, k43, k44, k45 }, \
    { k50, k51, k52, k53, k54, k55 }, \
    { k60, k61, k62, k63, k64, k65 }, \
    { k70, k71, k72, k73, k74, k75 }, \
}

#define LAYOUT_kc(\
    L00, L01, L02, L03, L04, L05, \
    L10, L11, L12, L13, L14, L15, \
    L20, L21, L22, L23, L24, L25, \
    L30, L31, L32, L33, L34, L35, \
    L45, L44, L43, L42, L41, L40, \
    L55, L54, L53, L52, L51, L50, \
    L65, L64, L63, L62, L61, L60, \
    L75, L74, L73, L72, L71, L70 \
) { \
    { KC_##L00, KC_##L01, KC_##L02, KC_##L03, KC_##L04, KC_##L05 }, \
    { KC_##L10, KC_##L11, KC_##L12, KC_##L13, KC_##L14, KC_##L15 }, \
    { KC_##L20, KC_##L21, KC_##L22, KC_##L23, KC_##L24, KC_##L25 }, \
    { KC_##L30, KC_##L31, KC_##L32, KC_##L33, KC_##L34, KC_##L35 }, \
    { KC_##L40, KC_##L41, KC_##L42, KC_##L43, KC_##L44, KC_##L45 }, \
    { KC_##L50, KC_##L51, KC_##L52, KC_##L53, KC_##L54, KC_##L55 }, \
    { KC_##L60, KC_##L61, KC_##L62, KC_##L63, KC_##L64, KC_##L65 }, \
    { KC_##L70, KC_##L71, KC_##L72, KC_##L73, KC_##L74, KC_##L75 }, \
}

#define LAYOUT LAYOUT_all
#define KEYMAP LAYOUT_all

#endif
