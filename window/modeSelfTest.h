#ifndef MODESELFTEST_H
#define MODESELFTEST_H

#include <Arduino.h>
#include <Ticker.h>
#include "leds.h"
#include "mode.h"

/**
 * @brief Self test mode; test each LED strip
 * 
 */
class ModeSelfTest : public Mode
{
public:
    /**
     * @brief Self-test mode cannot be configured
     * @param payload Message payload
     * @param length Message length
     */
    void onConfigMessage(byte *payload, unsigned int length) override{};

protected:
    /**
     * @brief Runs on mode start.
     */
    void onStart() override;

    /**
     * @brief Runs on mode stop.
     */
    void onStop() override;

private:
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