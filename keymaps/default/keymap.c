#include QMK_KEYBOARD_H
#include <qp.h>
#include "print.h"
#include "image/my_image.qgf.h"
#include "image/ekbox.qgf.h"
#include "font/arial.qff.h"

enum custom_layers {
    _BASE = 0,   // 0 BASE
    _NAV = 1,    // 1 NAV
    _NUM = 2,    // 2 NUM
    _SYM = 3,    // 3 SYM
    _MOUSE = 4,  // 4 MOUSE
    _EXTRA = 5   // 5 EXTRA
};

// Thumb cluster layer-taps
#define NAV_SPC  LT(_NAV, KC_SPC)
#define NUM_ENT  LT(_NUM, KC_ENT)
#define SYM_BSP  LT(_SYM, KC_BSPC)
#define MSE_TAB  LT(_MOUSE, KC_TAB)
#define EXT_ESC  LT(_EXTRA, KC_ESC)

// Define Home Row Mods for cleaner code (Optional but recommended)
#define GUI_A   LGUI_T(KC_A)
#define ALT_S   LALT_T(KC_S)
#define CTL_D   LCTL_T(KC_D)
#define SFT_F   LSFT_T(KC_F)

#define SFT_J   RSFT_T(KC_J)
#define CTL_K   RCTL_T(KC_K)
#define ALT_L   RALT_T(KC_L)
#define GUI_SCL RGUI_T(KC_SCLN)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* 0 BASE (QWERTY with Home Row Mods)
   * ,----------------------------------.      ,----------------------------------.
   * |  Q  |  W  |  E  |  R  |  T  |      |  Y  |  U  |  I  |  O  |  P  |
   * |GUI/A|ALT/S|CTL/D|SFT/F|  G  |      |  H  |SFT/J|CTL/K|ALT/L|GUI/;|
   * |  Z  |  X  |  C  |  V  |  B  |      |  N  |  M  |  ,  |  .  |  /  |
   * `----------------------------------'      `----------------------------------'
   *             | GUI | MSE/TAB| NAV/SPC|    | NUM/ENT| SYM/BSP| EXTRA/ESC |
   */
  [_BASE] = LAYOUT_split_3x5_3(
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    GUI_A,   ALT_S,   CTL_D,   SFT_F,   KC_G,       KC_H,    SFT_J,   CTL_K,   ALT_L,   GUI_SCL,
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                      KC_LGUI, MSE_TAB, NAV_SPC,    NUM_ENT, SYM_BSP, EXT_ESC
  ),

  /* 1 NAV (Navigation & Editing)
   * ,----------------------------------.      ,----------------------------------.
   * |     |     |     |     |     |      | PgUp| Home| Up  | End |     |
   * |Shift|Ctrl | Alt | GUI |     |      | PgDn| Left| Down|Right|     |
   * |Undo | Cut |Copy |Paste|     |      |     |     |     |     |     |
   * `----------------------------------'      `----------------------------------'
   *             |     |     |        |    |        | Delete|     |
   */
  [_NAV] = LAYOUT_split_3x5_3(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PGUP, KC_HOME, KC_UP,   KC_END,  XXXXXXX,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,
    LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                      _______, _______, _______,    _______, KC_DEL,  _______
  ),

  /* 2 NUM (Numpad)
   * ,----------------------------------.      ,----------------------------------.
   * |     |     |     |     |     |      |  /  |  7  |  8  |  9  |  -  |
   * |Shift|Ctrl | Alt | GUI |     |      |  *  |  4  |  5  |  6  |  +  |
   * |     |     |     |     |     |      |  0  |  1  |  2  |  3  |  =  |
   * `----------------------------------'      `----------------------------------'
   *             |     |     |        |    |        |       |  .  |
   */
  [_NUM] = LAYOUT_split_3x5_3(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PSLS, KC_7,    KC_8,    KC_9,    KC_PMNS,
    KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PAST, KC_4,    KC_5,    KC_6,    KC_PPLS,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_0,    KC_1,    KC_2,    KC_3,    KC_EQL,
                      _______, _______, _______,    _______, _______, KC_DOT
  ),

  /* 3 SYM (Symbols)
   * ,----------------------------------.      ,----------------------------------.
   * |  !  |  @  |  #  |  $  |  %  |      |  ^  |  &  |  *  |  (  |  )  |
   * |  \  |  |  |  -  |  _  |     |      |     |  =  |  +  |  {  |  }  |
   * |  `  |  ~  |     |     |     |      |     |     |     |  [  |  ]  |
   * `----------------------------------'      `----------------------------------'
   *             |     |     |        |    |        |       |     |
   */
  [_SYM] = LAYOUT_split_3x5_3(
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
    KC_BSLS, KC_PIPE, KC_MINS, KC_UNDS, XXXXXXX,    XXXXXXX, KC_EQL,  KC_PPLS, KC_LCBR, KC_RCBR,
    KC_GRV,  KC_TILD, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC,
                      _______, _______, _______,    _______, _______, _______
  ),

  /* 4 MOUSE (Trackball & Mouse Clicks)
   */
  [_MOUSE] = LAYOUT_split_3x5_3(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MS_WH_UP,                XXXXXXX, KC_MS_WH_UP, KC_MS_UP,  KC_MS_WH_DWON , XXXXXXX, 
    KC_MS_BTN4, KC_MS_BTN3, KC_MS_BTN2, KC_MS_BTN1, KC_MS_WH_DOWN,   XXXXXXX,  KC_MS_LEFT,  KC_MS_DOWN, KC_MS_RIGHT,   XXXXXXX, 
    KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX,                XXXXXXX,  XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,
                      _______, _______, _______,    _______,     _______,    _______
  ),

  /* 5 EXTRA (F-Keys, Media, Board Settings)
   * ,----------------------------------.      ,----------------------------------.
   * | F1  | F2  | F3  | F4  | F5  |      | F6  | F7  | F8  | F9  | F10 |
   * | F11 | F12 |     |     |     |      |VolUp| Mute|VolDn|     |     |
   * |BT1  |BT2  |BT3  |BT CLR|    |      |Prev |Play |Next |     |Reset|
   * `----------------------------------'      `----------------------------------'
   *             |     |     |        |    |        |       |     |
   */
  [_EXTRA] = LAYOUT_split_3x5_3(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
    KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,    KC_VOLU, KC_MUTE, KC_VOLD, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, QK_BOOT,
                      _______, _______, _______,    _______, _______, _______
  )
};

void pointing_device_init_user(void) {
    set_auto_mouse_enable(true);         // always required before the auto mouse feature will work
}

static bool is_v_scroll_active = false;
static bool is_h_scroll_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true; // Keep standard key processing intact
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    
    // Check modifier states after key events resolve
    is_v_scroll_active = (mods & MOD_MASK_SHIFT);
    is_h_scroll_active = (mods & MOD_MASK_CTRL);
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // If Shift is held, scroll vertically
    if (is_v_scroll_active) {
        mouse_report.v = -mouse_report.y; // Vertical scroll (y-axis)
        mouse_report.x = 0;               // Block cursor X
        mouse_report.y = 0;               // Block cursor Y
    } 
    // If Ctrl is held, scroll horizontally
    else if (is_h_scroll_active) {
        mouse_report.h = mouse_report.x;  // Horizontal scroll (x-axis)
        mouse_report.x = 0;               // Block cursor X
        mouse_report.y = 0;               // Block cursor Y
    }
    
    return mouse_report;
}

static painter_image_handle_t testimg;
static painter_device_t display;
//static painter_font_handle_t my_font;

enum HSL
{
    HUE,
    SAT,
    VAL,
};

const uint8_t black[] = {0,   0,   0};
const uint8_t white[] = {0,   0,   255};
const uint8_t red[]   = {0,   255, 127};
const uint8_t blue[]  = {170, 255, 127};

void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_matrix = true;
    debug_keyboard=true;

    dprint("display_init_kb - start\n");
    // LCDのBACKLIGHT PINをHIGHにする
    setPinOutput(LCD_BACKLIGHT_PIN);
    writePinLow(LCD_BACKLIGHT_PIN);

    display = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, SPI_CS_PIN, LCD_DC_PIN, LCD_RESET_PIN, SPI_DIVISOR, SPI_MODE );

    qp_init(display, QP_ROTATION_0);  // Adjust rotation: 0, 90, 180, or 270
    qp_power(display, true);          // Enable power/backlight if needed
    qp_clear(display);                // Clear screen on startup
    
    backlight_enable();
    backlight_level(1); // Sets backlight to level 3

    // 背景描画
    //qp_rect(display, startPos[X], startPos[Y], endPos[X], endPos[Y], black[HUE], black[SAT], black[VAL], true);
    
    // 5. 描画
    // qp_rect(display, 0, 0, 135, 240, white[HUE], white[SAT], white[VAL], true);
    // qp_circle(display, 100, 100, 100, black[HUE], black[SAT], black[VAL], true);
    // qp_line(display, 120, 0, 120, 240, red[HUE], red[SAT], red[VAL]);
    // qp_line(display, 0, 120, 240, 120, blue[HUE], blue[SAT], blue[VAL]);
     qp_rect(display,LCD_OFFSETX,LCD_OFFSETY,240, 340, 0, 0, 0, true);
    // Flush changes to the display
    qp_flush(display);

    testimg = qp_load_image_mem(gfx_ekbox);
    if (testimg != NULL) {
        qp_drawimage(display, LCD_OFFSETX, LCD_OFFSETY, testimg);
    }

    // Draw the text (device, x, y, font_handle, text)
    // my_font = qp_load_font_mem(font_arial);
    // if (my_font != NULL) {
    //     static const char *text = "EKBOX";
    //     int16_t width = qp_textwidth(my_font, text);
    //     qp_drawtext(display, (135 - width), (240 - my_font->line_height), my_font, text); 
    // }

}
