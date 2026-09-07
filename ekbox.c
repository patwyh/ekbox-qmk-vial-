// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ekbox.h"
#include "display.h"
#include "pointing_device.h"
#include "print.h"

static bool display_enabled;
static uint16_t last_keycode = KC_NO;
volatile uint32_t last_trackball_activity = 0;

/* public function to be used in keymaps */
bool is_display_enabled(void) {
    return display_enabled;
}

// Track trackball movement
report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    if (mouse_report.x != 0 || mouse_report.y != 0 || mouse_report.buttons != 0) {
        last_trackball_activity = timer_read32(); // Record movement time
    }
    return pointing_device_task_user(mouse_report);
}

// Intercept layer state changes and update display
layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);
    if (display_enabled) {
        display_process_layer(state);
    }
    return state;
}

// /* Caps Lock processing */
// bool led_update_kb(led_t led_state) {
//     bool res = led_update_user(led_state);
//     if (res && display_enabled) {
//         display_process_caps(led_state.caps_lock);
//     }

//     return res;
// }

void housekeeping_task_kb(void) {
    if (display_enabled) {
        display_housekeeping_task();
    }
}

void keyboard_post_init_kb(void) {
    display_enabled = false;

    if (is_keyboard_left()) {
        display_enabled = display_init_kb();
        display_process_layer(layer_state); // Update display with current layer state
    }

    keyboard_post_init_user();
}

uint16_t get_last_pressed_keycode(void) {
    return last_keycode;
}

// Track pressed states for matrix (0,0) and (0,1)
static bool key_0_0_pressed = false;
static bool key_0_1_pressed = false;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // Read the matrix row and column directly
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;

    dprintf("Keycode: %u, Row: %u, Col: %u, Pressed: %s\n", keycode, row, col, record->event.pressed ? "true" : "false");

    // Check if the event is happening on Left Keyboard matrix positions (0,0) or (0,1)
    if (row == 0 && col == 0) {
        key_0_0_pressed = record->event.pressed;
        if (key_0_0_pressed && key_0_1_pressed && display_enabled) {
            toggle_wpm_arc_visibility();
        }
    } else if (row == 0 && col == 1) {
        key_0_1_pressed = record->event.pressed;
        if (key_0_0_pressed && key_0_1_pressed && display_enabled) {
            toggle_wpm_arc_visibility();
        }
    }

    if (!process_record_user(keycode, record)) {
        return false;
    }
    
    // Capture basic keypresses on key down (ignoring modifier keys themselves)
    if (record->event.pressed && keycode < QK_MODS) {
        last_keycode = keycode;
    }
    return true;
}

// Optional callback if you want to perform custom display/state actions on trackball motion
// void pointing_device_task_user(report_pointing_device_t mouse_report) {
//     // QMK handles AUTO_MOUSE_DEFAULT_LAYER activation internally.
//     // layer_state_set_kb will be automatically called when the layer shifts.
// }

