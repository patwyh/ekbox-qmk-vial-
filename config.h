#pragma once

#define SERIAL_USART_TX_PIN GP12
#define SERIAL_USART_RX_PIN GP13
#define SERIAL_USART_PIN_SWAP
#define SERIAL_USART_FULL_DUPLEX

#define USB_VBUS_PIN GP11

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP24
#define I2C1_SCL_PIN GP25

#define MATRIX_ROWS 8
#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS 5

#define MASTER_LEFT
//#define EE_HANDS

/* RP2040- and hardware-specific config */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64

// // Trackpad configuration 
// #define CIRQUE_PINNACLE_DIAMETER_MM 23 //23 //35 //40

// // Optional enhancements
// #define CIRQUE_PINNACLE_TAP_ENABLE                  // Enable single-tap for left-click
// #define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE        // Enable secondary tap (e.g., right-click)
// #define POINTING_DEVICE_GESTURES_SCROLL_ENABLE      // Enable edge/circular scrolling
// #define POINTING_DEVICE_ROTATION_90                 // Adjust if the trackpad orientation requires rotation (common in split keyboards) default left, 270 top

// #define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE              // (Optional) Enable inertial cursor. Cursor continues moving after a flick gesture and slows down by kinetic friction.

// // Chip Select pin for the trackpad
// #define CIRQUE_PINNACLE_SPI_CS_PIN GP17 
// #define CIRQUE_PINNACLE_SPI_CS_PIN_RIGHT GP17

// Trackball configuration
// Set the layer activated when moving the trackball
#define PMW33XX_CS_PIN      GP17    
#define PMW3389_CPI         1000  // Sets default tracking speed (CPI range: 50-16000 
#define POINTING_DEVICE_INVERT_Y
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 4 
#define AUTO_MOUSE_TIME 650     // Timeout in milliseconds before returning to the previous layer

#define POINTING_DEVICE_RIGHT  //POINTING_DEVICE_COMBINED
#define POINTING_DEVICE_TASK_THROTTLE_MS 1
#define POINTING_DEVICE_DRAG_SCROLL_ENABLE
#define POINTING_DEVICE_DRAG_SCROLL_DOWNSCALE 8
#define SPLIT_POINTING_ENABLE

// ST7789 Display configuration
#define ST7789_NUM_DEVICES 1
#define QUANTUM_PAINTER_NUM_DEVICES 1  // Default is 1; increase for multiple displays

/* SPI Setting */
#define SPI_DRIVER      SPID0
#define SPI_CS_PIN      GP15     // Chip Select
#define SPI_SCK_PIN     GP18     // Clock
#define SPI_MOSI_PIN    GP19     // Master -> Slave
#define SPI_MISO_PIN    GP16     // Master <- Slave
#define SPI_DIVISOR     1    // SPI Clock Divisor
#define SPI_MODE        3

/* LCD setting */
#define LCD_RESET_PIN       GP24
#define LCD_DC_PIN          GP25
#define LCD_BACKLIGHT_PIN   GP10
#define LCD_HEIGHT          320//240
#define LCD_WIDTH           172
#define LCD_OFFSETX         34
#define LCD_OFFSETY         0
// #define LCD_HEIGHT          320//240
// #define LCD_WIDTH           135//172
// #define LCD_OFFSETX         52
// #define LCD_OFFSETY         40

#define BACKLIGHT_PWM_DRIVER PWMD5
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
//#define BACKLIGHT_PWM_COUNTER_FREQUENCY 1000000 

//#define BACKLIGHT_LEVELS 5     // Number of brightness steps (max 31)
#define BACKLIGHT_DEFAULT_LEVEL 3    // Default brightness level (out of 5)
#define BACKLIGHT_ON_STATE 0   // Use 1 for high-active (common) or 0 for low-active
#define BACKLIGHT_DEFAULT_ON true    // Ensure backlight is ON by default

#define QUANTUM_PAINTER_LVGL_USE_CUSTOM_CONF
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0

#define DEBUG_ACTION
#define OS_DETECTION_DEBUG_ENABLE