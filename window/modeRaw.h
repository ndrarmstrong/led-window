#ifndef MODERAW_H
#define MODERAW_H

#include <Arduino.h>
#include "leds.h"
#include "log.h"
#include "mode.h"

/**
 * @brief Raw mode; render value exactly as specified, without processing
 * 
 */
class ModeRaw : public Mode
{
public:
    /**
     * @brief Configure this mode
     * 
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     * @param dw Daylight white level
     */
    void configure(uint8_t r, uint8_t g, uint8_t b, int dw);

    /**
     * @brief Handle incoming message to configure mode
     * @param payload Message payload
     * @param length Message length
     */
    void onConfigMessage(byte *payload, unsigned int length) override;

protected:
    /**
     * @brief Reset state when stopped.
     */
    void onStop() override { reset(); };

private:
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