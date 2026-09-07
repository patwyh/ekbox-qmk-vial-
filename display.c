// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "display.h"
#include "qp.h"
#include "lvgl_helpers.h"
#include "wpm.h"
#include "timer.h" 

/* shared styles */
lv_style_t style_screen;
lv_style_t style_container;
lv_style_t style_button;
lv_style_t style_button_active;

/* screens */
static lv_obj_t *screen_home;

/* home screen content */
static lv_obj_t *label_shift;
static lv_obj_t *label_ctrl;
static lv_obj_t *label_alt;
static lv_obj_t *label_gui;
//static lv_obj_t *label_caps;
static lv_obj_t *label_wpm;
static lv_obj_t *arc_wpm; // Arc object reference
static lv_obj_t *label_layer;
static lv_obj_t *label_key_press;

// Track visibility state
static bool wpm_arc_visible = false;

#define WPM_MAX 140 // Maximum WPM target for full arc gauge
static uint8_t smoothed_wpm = 0; // Stores smoothed WPM state
#define TRACKBALL_IDLE_TIMEOUT 500

void toggle_wpm_arc_visibility(void) {
    if (!arc_wpm) return;
    
    wpm_arc_visible = !wpm_arc_visible;
    
    // Toggle LVGL hidden flag on arc_wpm
    toggle_hidden(arc_wpm, wpm_arc_visible);
}

// Maps WPM speed to a color hue transition (e.g., Cool Blue -> Green -> Vibrant Orange)
static lv_color_t get_wpm_color(uint8_t wpm) {
    if (wpm > WPM_MAX) wpm = WPM_MAX;
    // Smoothly shift hue from ~200 (light blue) down to 0 (red/orange) based on speed
    uint16_t hue = 200 - ((uint32_t)wpm * 200 / WPM_MAX);
    return lv_color_hsv_to_rgb(hue, 85, 100);
}

// Helper function to map layer index to a name
static const char *get_layer_name(uint8_t layer) {
    switch (layer) {
        case 0:  return "0 BASE";
        case 1:  return "1 NAV";
        case 2:  return "2 NUM";
        case 3:  return "3 SYM";
        case 4:  return "4 MOUSE";
        default: return "5 EXTRA";
    }
}

// Convert basic QMK keycodes to displayable characters/names
static const char *keycode_to_str(uint16_t keycode) {
    if (keycode >= KC_A && keycode <= KC_Z) {
        static char letter[2] = {0};
        letter[0] = 'A' + (keycode - KC_A);
        return letter;
    }
    if (keycode >= KC_1 && keycode <= KC_0) {
        static char num[2] = {0};
        num[0] = (keycode == KC_0) ? '0' : ('1' + (keycode - KC_1));
        return num;
    }
    
    switch (keycode) {
        case KC_SPACE: return "SPC";
        case KC_ENTER: return "ENT";
        case KC_BSPC:  return "BSPC";
        case KC_TAB:   return "TAB";
        case KC_ESC:   return "ESC";
        case KC_DOT:   return ".";
        case KC_COMM:  return ",";
        default:       return "---";
    }
}

// Maps each layer to a specific solid background color for the layer label
static lv_color_t get_layer_bg_color(uint8_t layer) {
    switch (layer) {
        case 0:  return lv_color_make(60, 60, 60);                  // Solid Charcoal/Gray for Base
        case 1:  return lv_color_make(0, 120, 255);                 // Solid Bright Blue for Nav
        case 2:  return lv_color_make(230, 40, 40);                 // Solid Bright Red for Num
        case 3:  return lv_color_make(40, 180, 70);                 // Solid Bright Green for Sym
        case 4:  return lv_color_make(160, 32, 240);                // Solid Bright Purple for Mouse
        default: return lv_color_make(100, 100, 100);               // Solid Light Gray for Fallback
    }
}

void display_process_layer(layer_state_t state) {
    uint8_t highest_layer = get_highest_layer(state);

    if (label_layer) {
        // Update layer label text
        lv_label_set_text(label_layer, get_layer_name(highest_layer));
        
        // Get layer solid color
        lv_color_t bg_color = get_layer_bg_color(highest_layer);

        // Apply solid background color exclusively to the layer label button
        lv_obj_set_style_bg_color(label_layer, bg_color, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(label_layer, LV_OPA_COVER, LV_PART_MAIN);

        // Optional: Change label text to white for maximum contrast against solid colors
        lv_obj_set_style_text_color(label_layer, lv_color_white(), LV_PART_MAIN);
    }
}

void init_styles(void) {
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, lv_color_black());

    lv_style_init(&style_container);
    lv_style_set_pad_top(&style_container, 0);
    lv_style_set_pad_bottom(&style_container, 0);
    lv_style_set_pad_left(&style_container, 0);
    lv_style_set_pad_right(&style_container, 0);
    lv_style_set_bg_opa(&style_container, 0);
    lv_style_set_border_width(&style_container, 0);
    lv_style_set_width(&style_container, lv_pct(100));
    lv_style_set_height(&style_container, LV_SIZE_CONTENT);

    lv_style_init(&style_button);
    lv_style_set_pad_top(&style_button, 4);
    lv_style_set_pad_bottom(&style_button, 4);
    lv_style_set_pad_left(&style_button, 4);
    lv_style_set_pad_right(&style_button, 4);
    lv_style_set_radius(&style_button, 6);
    lv_style_set_text_color(&style_button, lv_palette_main(LV_PALETTE_AMBER));

    lv_style_init(&style_button_active);
    lv_style_set_bg_color(&style_button_active, lv_palette_main(LV_PALETTE_AMBER));
    lv_style_set_bg_opa(&style_button_active, LV_OPA_100);
    lv_style_set_text_color(&style_button_active, lv_color_black());
}

void init_screen_home(void) {
    screen_home = lv_scr_act();

    // Ensure full screen background stays solid black
    lv_obj_set_style_bg_color(screen_home, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen_home, LV_OPA_COVER, 0);

    // Force negative right margin to cover off-screen frame buffer artifacts 
    //lv_obj_set_style_pad_right(screen_home, -10, 0);

    lv_obj_add_style(screen_home, &style_screen, 0);
    
    // Stack items tightly from top to bottom instead of stretching them apart
    // lv_obj_set_layout(screen_home, LV_LAYOUT_FLEX);
    // lv_obj_set_flex_flow(screen_home, LV_FLEX_FLOW_COLUMN);
    // lv_obj_set_flex_align(screen_home, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Zero out padding and gaps on the main screen container
    // lv_obj_set_style_pad_all(screen_home, 0, 0);
    // lv_obj_set_style_pad_row(screen_home, 2, 0);

    // Evenly distribute top modifiers, middle gauge, and bottom layer across 320px
    use_flex_column(screen_home);
    lv_obj_set_style_pad_all(screen_home, 8, 0);

    // --- Top Modifier Rows ---
    lv_obj_t *mods = lv_obj_create(screen_home);
    lv_obj_add_style(mods, &style_container, 0);
    use_flex_column(mods);
    lv_obj_set_style_pad_row(mods, 4, 0); 
    //lv_obj_set_style_pad_row(mods, 0, 0); // Remove gap between mod rows

    lv_obj_t *mods_row1 = lv_obj_create(mods);
    lv_obj_add_style(mods_row1, &style_container, 0);
    use_flex_row(mods_row1);
    label_gui = create_button(mods_row1, "GUI", &style_button, &style_button_active);
    label_alt = create_button(mods_row1, "ALT", &style_button, &style_button_active);

    lv_obj_t *mods_row2 = lv_obj_create(mods);
    lv_obj_add_style(mods_row2, &style_container, 0);
    use_flex_row(mods_row2);
    label_ctrl  = create_button(mods_row2, "CTL", &style_button, &style_button_active);
    label_shift = create_button(mods_row2, "SFT", &style_button, &style_button_active);

    // --- Middle WPM Dynamic Arc (Reduced from 130px to 90px) ---
    arc_wpm = lv_arc_create(screen_home);
    // Hide the WPM arc by default on startup
    if (arc_wpm) {
        toggle_hidden(arc_wpm, wpm_arc_visible);
    }
    //lv_obj_set_size(arc_wpm, 90, 90);
    lv_obj_set_size(arc_wpm, 140, 140);
    lv_arc_set_rotation(arc_wpm, 135);
    lv_arc_set_bg_angles(arc_wpm, 0, 270);
    lv_arc_set_range(arc_wpm, 0, WPM_MAX);
    lv_arc_set_value(arc_wpm, 0);

    lv_obj_remove_style(arc_wpm, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc_wpm, LV_OBJ_FLAG_CLICKABLE);
    
    // Remove arc padding so it doesn't take invisible extra height
    lv_obj_set_style_pad_all(arc_wpm, 0, 0);

    lv_obj_set_style_arc_width(arc_wpm, 12, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc_wpm, lv_color_make(40, 40, 40), LV_PART_MAIN);

    lv_obj_set_style_arc_width(arc_wpm, 12, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc_wpm, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_PART_INDICATOR);

    // WPM Label inside arc
    label_wpm = lv_label_create(arc_wpm);
    lv_label_set_text(label_wpm, "0");
    lv_obj_set_style_text_align(label_wpm, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(label_wpm);
    // Use standard/smaller font so it fits inside 90px arc
    lv_obj_set_style_text_font(label_wpm, LV_FONT_DEFAULT, LV_PART_MAIN);

    //label_caps = create_button(screen_home, "CAPS", &style_button, &style_button_active);
    label_layer = create_button(screen_home, "0 BASE", &style_button, &style_button_active);

    // Label for displaying active modifiers + last pressed key
    label_key_press = lv_label_create(screen_home);
    lv_label_set_text(label_key_press, "KEY: NONE");
    #if LV_FONT_MONTSERRAT_28
        lv_obj_set_style_text_font(label_key_press, &lv_font_montserrat_28, LV_PART_MAIN);
    #endif
}

bool display_init_kb(void) {
    dprint("display_init_kb - start\n");

    backlight_enable();

    // painter_device_t display = qp_st7789_make_spi_device(240, 300, SPI_CS_PIN, LCD_DC_PIN, LCD_RESET_PIN, 16, 3);
    // qp_set_viewport_offsets(display, 0, 20);

    // 1. Pass width = 135, height = 240 to the display initializer
    painter_device_t display = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, SPI_CS_PIN, LCD_DC_PIN, LCD_RESET_PIN, 16, 3);

    // 2. Adjust offsets (Standard 135x240 ST7789 modules require X offset = 52 or 53, Y offset = 40)
    qp_set_viewport_offsets(display, LCD_OFFSETX, LCD_OFFSETY);

    if (!qp_init(display, QP_ROTATION_0) || !qp_power(display, true) || !qp_lvgl_attach(display)) return false;

    dprint("display_init_kb - initialised\n");

    lv_disp_t  *lv_display = lv_disp_get_default();
    lv_theme_t *lv_theme   = lv_theme_default_init(lv_display, lv_palette_main(LV_PALETTE_AMBER), lv_palette_main(LV_PALETTE_BLUE), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(lv_display, lv_theme);
    init_styles();

    bool res = display_init_user();
    if (res) {
        dprint("display_init_kb - adding default home screen\n");
        init_screen_home();
    }

    return true;
}

__attribute__((weak)) bool display_init_user(void) {
    return true;
}

__attribute__((weak)) void display_housekeeping_task(void) {
    // 1. Completely bypass display updates if trackball was recently active
    if (timer_elapsed32(last_trackball_activity) < TRACKBALL_IDLE_TIMEOUT) {
        return; 
    }

    // Throttle UI updates to ~30 FPS (33ms) to free CPU for trackball polling
    static uint16_t wpm_timer = 0;
    if (timer_elapsed(wpm_timer) < 100) {
        return;
    }

    toggle_state(label_shift, LV_STATE_PRESSED, MODS_SHIFT);
    toggle_state(label_ctrl, LV_STATE_PRESSED, MODS_CTRL);
    toggle_state(label_alt, LV_STATE_PRESSED, MODS_ALT);
    toggle_state(label_gui, LV_STATE_PRESSED, MODS_GUI);

    wpm_timer = timer_read();

    uint8_t raw_wpm = get_current_wpm();
    if (raw_wpm <= 2) raw_wpm = 0;
    smoothed_wpm = (smoothed_wpm * 3 + raw_wpm) / 4;

    // 3. Only trigger UI update if smoothed value actually changes
    static uint8_t last_displayed_wpm = 255;
    if (arc_wpm && label_wpm && smoothed_wpm != last_displayed_wpm) {
        last_displayed_wpm = smoothed_wpm;

        lv_arc_set_value(arc_wpm, smoothed_wpm);
        
        lv_color_t dynamic_color = get_wpm_color(smoothed_wpm);
        lv_obj_set_style_arc_color(arc_wpm, dynamic_color, LV_PART_INDICATOR);
        
        lv_label_set_text_fmt(label_wpm, "%u", smoothed_wpm);
        lv_obj_center(label_wpm);
    }

    if (label_key_press) {
        uint8_t mods = get_mods() | get_oneshot_mods();
        uint16_t key = get_last_pressed_keycode();
        
        // Format modifier prefix dynamically
        char mod_str[32] = "";
        if (mods & MOD_MASK_CTRL)  strcat(mod_str, "C-");
        if (mods & MOD_MASK_ALT)   strcat(mod_str, "A-");
        if (mods & MOD_MASK_SHIFT) strcat(mod_str, "S-");
        if (mods & MOD_MASK_GUI)   strcat(mod_str, "G-");

        lv_label_set_text_fmt(label_key_press, "%s%s", mod_str, keycode_to_str(key));
    }
}

// __attribute__((weak)) void display_process_caps(bool active) {
//     dprint("display_process_caps\n");
//     toggle_state(label_caps, LV_STATE_PRESSED, active);
// }


