#include QMK_KEYBOARD_H

static bool encoder_tick = false; // Variable for tracking full and half cut-offs
static uint16_t saved_hue = 0;
static uint8_t saved_sat = 0;
static uint8_t saved_val = 0;
static bool is_layer0_active = true;
static bool is_layer2_active = true;

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
        _______, XXXXXXX, KC_BTN4, KC_BTN3, KC_BTN5, XXXXXXX, _______,         _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, RGB_HUD,
        _______, XXXXXXX, KC_BTN2, KC_MS_U, KC_BTN1, XXXXXXX,                           XXXXXXX, XXXXXXX, KC_WH_U, XXXXXXX, RGB_SAI, RGB_SAD,
        _______, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,                           XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_R, RGB_VAI, RGB_VAD,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DB_TOGG,         QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG, _______,
                                   _______, _______, _______, _______,         _______, _______, _______, _______ 
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (layer_state_is(0)) {
        if (index == 1) {
            if (encoder_tick) {
                tap_code(clockwise ? KC_RGHT : KC_LEFT);
            }
            encoder_tick = !encoder_tick;
            return false;
        }
        else if (index == 0) {
            tap_code(clockwise ? KC_WH_U : KC_WH_D);
        }
    }

    else if (layer_state_is(1)) {
        if (index == 1) {
            tap_code(clockwise ? KC_VOLD : KC_VOLU);
        }
        else if (index == 0) {
            tap_code(clockwise ? KC_WH_R : KC_WH_L);
        }
    }

    else if (layer_state_is(2)) {
        if (index == 1) {
            (clockwise ? rgblight_decrease_val : rgblight_increase_val)();
        }
        else if (index == 0) {
            (clockwise ? rgblight_increase_hue : rgblight_decrease_hue)();
        }
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t new_layer = get_highest_layer(state);

    if ((is_layer0_active && new_layer != 0) || (is_layer2_active && new_layer != 2)) {
        saved_hue = rgblight_get_hue();
        saved_sat = rgblight_get_sat();
        saved_val = rgblight_get_val();
        is_layer0_active = false;
        is_layer2_active = false;
    }

    if ((!is_layer0_active && new_layer == 0) || (!is_layer2_active && new_layer == 2)) {
        rgblight_sethsv(saved_hue, saved_sat, saved_val);
        is_layer0_active = true;
        is_layer2_active = true;
    }

    if (new_layer == 1) {
    rgblight_sethsv(255, 255, 127);
    }

    return state;
}