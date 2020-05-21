#ifndef MODESELFTEST_H
#define MODESELFTEST_H

#include <Arduino.h>
#include <Ticker.h>
#include "leds.h"

/**
 * @brief Self test mode; test each LED strip
 * 
 */
class ModeSelfTest
{
public:
    /**
     * @brief Construct a new Mode Self Test object
     * @param leds Leds instance
     */
    ModeSelfTest(Leds *leds);

    /**
     * @brief Start running self test mode.
     */
    void start();

    /**
     * @brief Stop running self test mode.
     */
    void stop();

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
     * @brief Ticker for alternating channels
     */
    Ticker ticker;

    /**
     * @brief Counter for channel phase
     */
    int phase = 0;

    /**
     * @brief Render test value to strip
     */
    void render();
};

#endif