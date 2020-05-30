#ifndef MODERAW_H
#define MODERAW_H

#include <Arduino.h>
#include "leds.h"
#include "log.h"

/**
 * @brief Raw mode; render value exactly as specified, without processing
 * 
 */
class ModeRaw
{
public:
    ModeRaw(Leds *leds);

    /**
     * @brief Start running raw mode.
     */
    void start();

    /**
     * @brief Stop running raw mode.
     */
    void stop();

    /**
     * @brief Configure this mode from MQTT message
     * 
     * @param topic MQTT topic
     * @param payload MQTT payload
     */
    void configure(char *topic, char *payload);

    /**
     * @brief Configure this mode from parsed message
     * 
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     * @param dw Daylight white level
     */
    void configure(uint8_t r, uint8_t g, uint8_t b, int dw);

private:
    /**
     * @brief Whether this module is enabled.
     */
    bool enabled = false;

    /**
     * @brief Leds instance.
     */
    Leds *leds;

    /**
     * @brief Active red value
     */
    uint8_t r;

    /**
     * @brief Active green value
     */
    uint8_t g;

    /**
     * @brief Active blue value
     */
    uint8_t b;

    /**
     * @brief Active daylight white value
     */
    uint8_t dw;

    /**
     * @brief Reset values to default
     */
    void reset();

    /**
     * @brief Render active value to the LED strips
     */
    void render();
};

#endif