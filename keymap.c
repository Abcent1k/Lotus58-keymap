#include QMK_KEYBOARD_H
#include "rgblight.h"
#include "eeprom.h" // Подключаем EEPROM библиотеку

static bool encoder_tick = false; // Variable for tracking full and half cut-offs
static bool snap_tap_function = false; // Variable for snap-tap function state
static bool mod_swap_function = false; // Variable for mod-swap function state

static bool invert_left_encoder = true; // Variable for inverting the left encoder
static bool invert_right_encoder = false; // Variable for inverting the right encoder

#define SNAP_TAP_EEPROM_ADDR 32 // Address in EEPROM for storing the snap-tap status
#define MOD_SWAP_EEPROM_ADDR 33 // Address in EEPROM for storing the mod-swap status


enum custom_keycodes {
    SNP_TAP = SAFE_RANGE,  // Define pseudo-key for snap-tap function
    MOD_SWP
};

//  -----------------------------  LEDs positions  -----------------------------
// | 5  | 4  | 3  | 2  | 1  | 0  |                | 29 | 30 | 31 | 32 | 33 | 34 |
// | 6  | 7  | 8  | 9  | 10 | 11 |                | 40 | 39 | 38 | 37 | 36 | 35 |
// | 17 | 16 | 15 | 14 | 13 | 12 |  ----    ----  | 41 | 42 | 43 | 44 | 45 | 46 |
// | 18 | 19 | 20 | 21 | 22 | 23 | | 24 |  | 53 | | 52 | 51 | 50 | 49 | 48 | 47 |
//  --------- | 28 | 27 | 26 | 25 | ----    ---- | 54 | 55 | 56 | 57 | --------- 
//             -------------------                ------------------- 

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
    {52, 1, HSV_RED},
    {53, 1, HSV_RED},
    {29, 1, HSV_TEAL},
    {30, 1, HSV_TEAL},
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

// Light LED when win swap to ctrl (mod_swap) is enable
const rgblight_segment_t PROGMEM mod_swap_layer_on[] = RGBLIGHT_LAYER_SEGMENTS(
    {30, 1, HSV_TEAL}
);

// Define the array of layers
const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer1,
    layer2,
    capslock_layer,
    snap_tap_layer_on,
    mod_swap_layer_on
);

// Enable the LED layers
void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
    snap_tap_function = eeprom_read_byte((void*)SNAP_TAP_EEPROM_ADDR); // Reading snap-tap status from EEPROM
    rgblight_set_layer_state(3, snap_tap_function); // Layer setting depending on the condition
    mod_swap_function = eeprom_read_byte((void*)MOD_SWAP_EEPROM_ADDR); // Reading snap-tap status from EEPROM
    rgblight_set_layer_state(4, mod_swap_function); // Layer setting depending on the condition
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
        _______, XXXXXXX, KC_BTN4, KC_BTN3, KC_BTN5, XXXXXXX, _______,         _______, SNP_TAP, MOD_SWP, XXXXXXX, XXXXXXX, RGB_HUI, RGB_HUD,
        _______, XXXXXXX, KC_BTN2, KC_MS_U, KC_BTN1, XXXXXXX,                           XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, RGB_SAI, RGB_SAD,
        _______, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,                           XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_R, RGB_VAI, RGB_VAD,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DB_TOGG,         QK_BOOT, KC_MPLY, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG, _______,
                                   _______, _______, _______, _______,         _______, _______,  _______, _______ 
    )
};

// Encoders
bool encoder_update_user(uint8_t index, bool clockwise) {
    bool left_clockwise = invert_left_encoder ? !clockwise : clockwise;
    bool right_clockwise = invert_right_encoder ? !clockwise : clockwise;

    switch (get_highest_layer(layer_state)) {
        case 1:
            tap_code(index == 1 ? (right_clockwise ? KC_VOLD : KC_VOLU) 
                : (left_clockwise ? KC_WH_R : KC_WH_L));
            break;
        case 2:
            (index == 1 ? (right_clockwise ? rgblight_decrease_val : rgblight_increase_val)
                : (left_clockwise ? rgblight_increase_hue : rgblight_decrease_hue))();
            break;
        default:
        case 0:
            if (index == 1) {
                if (encoder_tick) {
                    tap_code(right_clockwise ? KC_RGHT : KC_LEFT);
                }
                encoder_tick = !encoder_tick;
            } else {
                tap_code(left_clockwise ? KC_WH_U : KC_WH_D);
            }
            break;
    }

    return false;
}

// SNAP-TAP
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool aHeld = false;
    static bool dHeld = false;

switch (keycode) {
        case SNP_TAP: // Custom key for toggling the snap-tap function
            if (record->event.pressed) {
                snap_tap_function = !snap_tap_function; // Toggle the function state
                rgblight_set_layer_state(3, snap_tap_function); // Activate or deactivate layer snap-tap
                eeprom_update_byte((void*)SNAP_TAP_EEPROM_ADDR, snap_tap_function); // Saving the status to EEPROM
            }
            return false; // Don't send the key press

        case KC_A:
            if (!snap_tap_function) return true; // If the function is disabled, process normally
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
            if (!snap_tap_function) return true; // If the function is disabled, process normally
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

        case MOD_SWP: // Custom key for toggling the mod_swap function
            if (record->event.pressed) {
                mod_swap_function = !mod_swap_function; // Toggle the function state
                rgblight_set_layer_state(4, mod_swap_function); // Activate or deactivate layer snap-tap
                eeprom_update_byte((void*)MOD_SWAP_EEPROM_ADDR, mod_swap_function); // Saving the status to EEPROM
            }
            return false; // Don't send the original key press

        case KC_LGUI:
            if (mod_swap_function) {
                if (record->event.pressed) {
                    register_code(KC_LCTL); // If the feature is active, than send LCTL
                } else {
                    unregister_code(KC_LCTL); // Release LCTL
                }
                return false; // Don't send the original key press
            }
            return true; // If the feature is not active, we treat LGUI as a regular key

        default:
            return true; // Process all other keys normally
    }
}