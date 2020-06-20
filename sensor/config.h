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
    static constexpr const char *const HOSTNAME = "window-sensor";

    /**
     * @brief Pin for the system LED.
     */
    static const int PIN_READY_LED = D7;

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
     * @brief MQTT interval message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_INTERVAL = "interval";

    /**
     * @brief MQTT light level message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_LIGHT_LEVEL = "lightLevel";

    /**
     * @brief MQTT lux sensor message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_LUX = "lux";

    /**
     * @brief MQTT rgb sensor message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_RGB = "rgb";

    /**
     * @brief MQTT mode message topic
     */
    static constexpr const char *const MQTT_MSG_TOPIC_MODE = "mode";

    //
    // Sensor configuration
    //

    /**
     * @brief Default publish interval for light level measurements (s)
     */
    static const int SENSOR_DEFAULT_PUBLISH_INTERVAL_S = 30;

    //
    // Lux sensor configuration
    //

    /**
     * @brief Default lux sensor gain
     */
    static constexpr const float LUX_DEFAULT_GAIN = 0.25;

    /**
     * @brief Default lux sensor integration time (ms)
     */
    static const int LUX_DEFAULT_INTEGRATION_MS = 100;

    /**
     * @brief I2C address of the lux sensor
     */
    static const int LUX_SENSOR_ADDR = 0x48;

    //
    // RGB sensor configuration
    //

    /**
     * @brief Default RGB sensor intensity range
     */
    static const int RGB_DEFAULT_INTENSITY_RANGE = 0x08;

    /**
     * @brief Default RGB sensor IR filtering offset
     */
    static const int RGB_DEFAULT_IR_FILTER_OFFSET = 0x00;

    /**
     * @brief Default RGB sensor IR filtering level
     */
    static const int RGB_DEFAULT_IR_FILTER_LEVEL = 0x3F;
};

#endif