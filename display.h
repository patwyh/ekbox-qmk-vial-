// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "qp_lvgl.h"
#include <stdint.h>

#define MODS_SHIFT ((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT)
#define MODS_CTRL ((get_mods() | get_oneshot_mods()) & MOD_MASK_CTRL)
#define MODS_ALT ((get_mods() | get_oneshot_mods()) & MOD_MASK_ALT)
#define MODS_GUI ((get_mods() | get_oneshot_mods()) & MOD_MASK_GUI)

/* shared styles */
extern lv_style_t style_screen;
extern lv_style_t style_container;
extern lv_style_t style_button;
extern lv_style_t style_button_active;

extern volatile uint32_t last_trackball_activity; // Activity timestamp

bool display_init_kb(void);
bool display_init_user(void);
void display_housekeeping_task(void);
void display_process_caps(bool active);
void display_process_layer(layer_state_t state);
// Update the external declaration
uint16_t get_last_pressed_keycode(void);
void toggle_wpm_arc_visibility(void);