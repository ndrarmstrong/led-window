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
     * @brief Whether to remap the white LEDs to the onboard LED 
     * (useful for PWM testing - low power, easier to see glitches)
     */
    static const bool REMAP_WHITE_ONBOARD = false;

    /**
     * @brief Pin for the system LED.
     */
    static const int PIN_READY_LED = REMAP_WHITE_ONBOARD ? 16 : D7;

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

    /**
     * @brief Watchdog interval; if the system stays disconnected for this long, reset.
     */
    static const int SYSTEM_WATCHDOG_INTERVAL_MS = 5 * 60 * 1000;

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
    // MQTT Configuration
    //

    /**
     * @brief MQTT broker address (domain name)
     */
    static constexpr const char *const MQTT_BROKER_ADDRESS = ENV_MQTT_BROKER_ADDRESS;

    /**
     * @brief MQTT broker port
     */
    static const int MQTT_BROKER_PORT = 1883;

    /**
     * @brief MQTT broker secret for the device
     */
    static constexpr const char *const MQTT_BROKER_SECRET = ENV_MQTT_BROKER_SECRET;

    /**
     * @brief MQTT broker connect timeout, in seconds
     */
    static const int MQTT_BROKER_CONNECT_TIMEOUT_S = 10;

    /**
     * @brief MQTT maxmimum message size
     * 
     * This defaults to 256B, likely to reduce memory consumption.  We're serializing JSON and sending log messages, so
     * expand to give us a bit more working space.
     */
    static const int MQTT_MESSAGE_SIZE_B = 1000;

    /**
     * @brief MQTT keepalive interval
     */
    static const int MQTT_KEEPALIVE_S = 60;

    /**
     * @brief Topic namespace for all MQTT messages
     */
    static constexpr const char *const MQTT_TOPIC_NAMESPACE = "ledwin";

    /**
     * @brief Topic suffix for all MQTT requests
     */
    static constexpr const char *const MQTT_TOPIC_REQ_SUFFIX = "/req";

    /**
     * @brief MQTT describe message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_DESCRIBE = "describe";

    /**
     * @brief MQTT sys message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_SYS = "sys";

    /**
     * @brief MQTT daylight message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_DAYLIGHT = "daylight";

    /**
     * @brief MQTT mode message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_MODE = "mode";

    /**
     * @brief MQTT raw message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_RAW = "raw";

    //
    // Leds configuration
    //

    /**
     * @brief Pin for the white strips MOSFET gate.
     */
    static const int PIN_WHITE_STRIP = REMAP_WHITE_ONBOARD ? D7 : D6;

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