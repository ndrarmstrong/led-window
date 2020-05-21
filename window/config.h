// Configuration settings

#ifndef CONFIG_H
#define CONFIG_H

#include "env.h"
#include <Arduino.h>

class Config
{
public:
    //
    // System configuration
    //

    /**
     * @brief System hostname
     */
    static constexpr const char *const HOSTNAME = "led-window";

    /**
     * @brief Pin for the system LED.
     */
    static const int PIN_READY_LED = D7;

    /**
     * @brief Pin for the temperature sensor
     */
    static const int PIN_TEMP_SENSOR = D4;

    /**
     * @brief Whether OTA updates can be enabled.
     */
    static const bool SYSTEM_OTA_ALLOWED = true;

    /**
     * @brief OTA password
     */
    static constexpr const char *const SYSTEM_OTA_PASSWORD = ENV_OTA_PASSWORD;

    //
    // Wi-Fi Configuration
    //

    /**
     * @brief Wi-Fi SSID
     */
    static constexpr const char *const WIFI_SSID = ENV_WIFI_SSID;

    /**
     * @brief Wi-Fi Credential
     */
    static constexpr const char *const WIFI_CREDENTIAL = ENV_WIFI_CREDENTIAL;

    //
    // Leds configuration
    //

    /**
     * @brief Pin for the white strips MOSFET gate.
     */
    static const int PIN_WHITE_STRIP = D6;

    /**
     * @brief Pin for the top color strip data.
     */
    static const int PIN_TOP_COLOR_DATA = D2;

    /**
     * @brief Pin for the bottom color strip data.
     */
    static const int PIN_BOTTOM_COLOR_DATA = D1;

    /**
     * @brief Number of LEDs in the top color strip.
     */
    static const int TOP_COLOR_LED_COUNT = 40;

    /**
     * @brief Number of LEDs in the bottom color strip.
     */
    static const int BOTTOM_COLOR_LED_COUNT = 40;

    /**
     * @brief Frequency to PWM pins at; used for white LEDs.
     * 
     * Higher frequencies use more CPU time.
     */
    static const int PWM_HZ = 613;

    /**
     * @brief Range to PWM pins at; used for white LEDs.
     * 
     * Matches the brightness range of the color LEDs.
     */
    static const int PWM_RANGE = 255; // 8 bit
};

#endif