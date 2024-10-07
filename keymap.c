#include QMK_KEYBOARD_H
#include "rgblight.h"

static bool encoder_tick = false; // Variable for tracking full and half cut-offs

enum custom_keycodes {
    SNP_TAP = SAFE_RANGE,  // Define pseudo-key for snap-tap function
};

//  -----------------------------  LEDs positions  -----------------------------
// | 5  | 4  | 3  | 2  | 1  | 0  |                | 29 | 30 | 31 | 32 | 33 | 34 |
// | 6  | 7  | 8  | 9  | 10 | 11 |                | 40 | 39 | 38 | 37 | 36 | 35 |
// | 17 | 16 | 15 | 14 | 13 | 12 |  ----    ----  | 41 | 42 | 43 | 44 | 45 | 46 |
// | 18 | 19 | 20 | 21 | 22 | 23 | | 24 |  | 53 | | 52 | 51 | 50 | 49 | 48 | 47 |
//  --------------| 28 | 27 | 26 |  ----    ----  | 56 | 55 | 54 |-------------- 
//                 --------------                  -------------- 

// Light LEDs for 1st layer
const rgblight_segment_t PROGMEM layer1[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 6, HSV_BLUE},
    {29, 6, HSV_BLUE},
    {38, 1, HSV_RED},
    {42, 3, HSV_RED},
    {24, 1, HSV_GREEN},
    {39, 3, HSV_GREEN},
    {48, 6, HSV_GREEN}
);

// Light LEDs for 2nd layer
const rgblight_segment_t PROGMEM layer2[] = RGBLIGHT_LAYER_SEGMENTS(
    {18, 1, HSV_PURPLE},
    {53, 1, HSV_RED},
    {29, 1, HSV_TEAL},
    {33, 4, HSV_BLUE},
    {45, 2, HSV_BLUE},
    {48, 1, HSV_RED},
    {42, 3, HSV_GREEN},
    {38, 1, HSV_GREEN}
);

// Light LED when caps lock is enable
const rgblight_segment_t PROGMEM capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {24, 1, HSV_RED}
);

// Light LED when snap-tap is enable
const rgblight_segment_t PROGMEM snap_tap_layer_on[] = RGBLIGHT_LAYER_SEGMENTS(
    {29, 1, HSV_TEAL}
);

// Define the array of layers
const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer1,
    layer2,
    capslock_layer,
    snap_tap_layer_on
);

// Enable the LED layers
void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(2, led_state.caps_lock);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, 1));
    rgblight_set_layer_state(1, layer_state_cmp(state, 2));
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,       KC_1,    KC_2,    KC_3,    KC_4,    KC_5, KC_MPLY,        KC_PSCR, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        KC_TAB,       KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
        KC_LSFT,      KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        LT(2,KC_GRV), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, KC_RBRC,        KC_EQL,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_DEL,
                                     KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,         RSFT_T(KC_ENT), KC_RALT, MO(1), KC_BSPC
    ),

    [1] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   _______,         _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, _______, _______, _______, _______, _______,                           KC_PGUP, KC_INS,  KC_UP,   XXXXXXX, XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______, _______,                           KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,
        _______, _______, _______, _______, _______, _______,  KC_CAPS,        KC_PSCR, KC_SCRL, KC_PAUS, KC_HOME, KC_END,  KC_BSLS, _______,
                                   _______, _______, _______,  _______,        _______, _______, KC_TRNS, _______ 
    ),

    [2] = LAYOUT(
        _______, XXXXXXX, KC_BTN4, KC_BTN3, KC_BTN5, XXXXXXX, _______,         _______, SNP_TAP, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, RGB_HUD,
        _______, XXXXXXX, KC_BTN2, KC_MS_U, KC_BTN1, XXXXXXX,                           XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, RGB_SAI, RGB_SAD,
        _______, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,                           XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_R, RGB_VAI, RGB_VAD,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DB_TOGG,         QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG, _______,
                                   _______, _______, _______, _______,         _______, _______,  _______, _______ 
    )
};

// Encoders
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case 1:
            tap_code(clockwise ? (index == 1 ? KC_VOLD : KC_WH_R) : (index == 1 ? KC_VOLU : KC_WH_L));
            break;
        case 2:
            (clockwise ? (index == 1 ? rgblight_decrease_val : rgblight_increase_hue)
                    : (index == 1 ? rgblight_increase_val : rgblight_decrease_hue))();
            break;
        default:
        case 0:
            if (index == 1) {
                if (encoder_tick) {
                    tap_code(clockwise ? KC_RGHT : KC_LEFT);
                }
                encoder_tick = !encoder_tick;
            } else {
                tap_code(clockwise ? KC_WH_U : KC_WH_D);
            }
            break;
    }

    return false;
}

// SNAP-TAP
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool aHeld = false;
    static bool dHeld = false;
    static bool toggleFunction = false; // Variable for function state

switch (keycode) {
        case SNP_TAP: // Custom key for toggling the function
            if (record->event.pressed) {
                toggleFunction = !toggleFunction; // Toggle the function state
            }
            rgblight_set_layer_state(3, toggleFunction); // Activate or deactivate layer snap-tap
            return false; // Don't send the key press

        case KC_A:
            if (!toggleFunction) return true; // If the function is disabled, process normally
            aHeld = record->event.pressed;
            if (dHeld && aHeld) {
                unregister_code(KC_D);
            }
            else if (dHeld && !aHeld) {
                unregister_code(KC_A);
                register_code(KC_D);
                return false; // Don't send the original key press
            }
            return true;

        case KC_D:
            if (!toggleFunction) return true; // If the function is disabled, process normally
            dHeld = record->event.pressed;
            if (aHeld && dHeld) {
                unregister_code(KC_A);
            }
            else if (aHeld && !dHeld) {
                unregister_code(KC_D);
                register_code(KC_A);
                return false; // Don't send the original key press
            }
            return true;

        default:
            return true; // Process all other keys normally
    }
}