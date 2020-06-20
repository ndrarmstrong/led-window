#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "config.h"
#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#include "SparkFunISL29125.h"
#include <Ticker.h>
#include <ArduinoJson.h>
#include "log.h"

class LightSensor
{
public:
    /**
     * @brief Returns a reference to the global light sensor manager.
     * @return The global light sensor manager
     */
    static LightSensor &get();

    /**
     * @brief Handle incoming interval messages changing publish interval
     * @param payload Message payload
     * @param length Message length
     */
    void onIntervalMessage(byte *payload, unsigned int length);

    /**
     * @brief Handle incoming light level messages requesting an immediate publish
     * @param payload Message payload
     * @param length Message length
     */
    void onLightLevelMessage(byte *payload, unsigned int length);

    /**
     * @brief Handle incoming lux messages configuring the lux sensor
     * @param payload Message payload
     * @param length Message length
     */
    void onLuxMessage(byte *payload, unsigned int length);

    /**
     * @brief Handle incoming rgb messages configuring the color sensor
     * @param payload Message payload
     * @param length Message length
     */
    void onRgbMessage(byte *payload, unsigned int length);

    /**
     * @brief Start periodic measurements
     */
    void start();

    /**
     * @brief Stop periodic measurements
     */
    void stop();

private:
    /**
     * @brief Private constructor - only one instance.
     */
    LightSensor();

    /**
     * @brief Whether (periodic) measurements are enabled
     */
    bool enabled = false;

    /**
     * @brief Publish interval for light level measurements (s)
     */
    int publishIntervalS = Config::SENSOR_DEFAULT_PUBLISH_INTERVAL_S;

    /**
     * @brief Lux sensor gain
     */
    float luxGain = Config::LUX_DEFAULT_GAIN;

    /**
     * @brief Lux sensor integration time (ms)
     */
    int luxIntegrationMs = Config::LUX_DEFAULT_INTEGRATION_MS;

    /**
     * @brief RGB sensor intensity range
     */
    int rgbIntensityRange = Config::RGB_DEFAULT_INTENSITY_RANGE;

    /**
     * @brief RGB sensor IR filtering offset
     */
    int rgbIrFilterOffset = Config::RGB_DEFAULT_IR_FILTER_OFFSET;

    /**
     * @brief RGB sensor IR filtering level
     */
    int rgbIrFilterLevel = Config::RGB_DEFAULT_IR_FILTER_LEVEL;

    /**
     * Periodic publish ticker
     */
    Ticker publishTicker;

    /**
     * @brief Delayed reconfigure ticker
     */
    Ticker reconfigureTicker;

    /**
     * @brief Lux sensor instance
     */
    SparkFun_Ambient_Light *luxSensor = new SparkFun_Ambient_Light(Config::LUX_SENSOR_ADDR);

    /**
     * @brief RGB sensor instance
     */
    SFE_ISL29125 rgbSensor;

    /**
     * @brief Lux sensor ALS level (in lux)
     */
    uint32_t luxLevel = 0;

    /**
     * @brief Lux sensor "white" level (in lux?)
     *
     */
    uint32_t luxWhiteLevel = 0;

    /**
     * @brief RGB sensor red level (arbitrary units)
     */
    uint16_t rgbRedLevel = 0;

    /**
     * @brief RGB sensor green level (arbitrary units)
     */
    uint16_t rgbGreenLevel = 0;

    /**
     * @brief RGB sensor blue level (arbitrary units)
     */
    uint16_t rgbBlueLevel = 0;

    /**
     * @brief (Re)configure the sensor and publish settings
     */
    void reconfigure();

    /**
     * @brief Reconfigure after delay to allow the current MQTT message to be sent
     */
    void delayedReconfigure();

    /**
     * @brief Read sensors, then publish (runs periodically)
     */
    void readAndPublish();

    /**
     * @brief Read sensors and update local values
     */
    void readSensors();

    /**
     * @brief Publish the current light level
     */
    void publishLightLevel();

    /**
     * @brief Publish the current lux config
     */
    void publishLux();

    /**
     * @brief Publish the current rgb config
     */
    void publishRgb();

    /**
     * @brief Publish the current publish interval
     */
    void publishInterval();
};

#endif