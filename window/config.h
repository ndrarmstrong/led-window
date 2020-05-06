// Configuration settings

#ifndef CONFIG_H
#define CONFIG_H

// Network hostname (used for various things)
#define HOSTNAME "led-window"

// Whether OTA updates enabled, ever
#define OTA_ENABLED true

// Whether OTA updates are enabled always
// or just when commanded.  Keep disabled when not developing.
#define OTA_DEBUG true

// Board configuration
#define PWM_HZ 400    // PWM frequency
#define PWM_RANGE 255 // PWM range - 8 bit

// Pin configuration
#define PIN_READY_LED 13        // D7
#define PIN_WHITE_STRIP 14      // D5
#define PIN_TOP_COLOR_DATA 5    // D1
#define PIN_BOTTOM_COLOR_DATA 4 // D2

// Strip configuration
#define TOP_COLOR_LED_COUNT 40
#define BOTTOM_COLOR_LED_COUNT 40

#endif