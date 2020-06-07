#ifndef MODESTRESSPWM_H
#define MODESTRESSPWM_H

#include <Arduino.h>
#include <Ticker.h>
#include <FastLED.h>
#include "system.h"
#include "mqtt.h"
#include "leds.h"
#include "mode.h"

/**
 * @brief PWM stress mode; used to resolve conflicts between analogWrite and FastLED on ESP8266.
 * 
 * This fades colors from 0-100% on the last row of each strip so we can watch for garbled data in
 * the rest of the strip; it alternates the white strip at 0 (no PWM) and 1 (PWM)
 */
class ModeStressPwm : public Mode
{
public:
    /**
     * @brief PWM mode cannot be configured
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
     * @brief How many frames per second the fade renders at.
     */
    static const int FRAMES_PER_SECOND = 12;

    /**
     * @brief How many seconds to fade over.
     */
    static const int FADE_FRAMES = FRAMES_PER_SECOND * 3;

    /**
     * @brief How many seconds to hold between fades.
     */
    static const int HOLD_FRAMES = FRAMES_PER_SECOND * 0.5;

    /**
     * @brief Frame counter, for timing animations.
     * 
     */
    unsigned long frameCounter = 0;

    /**
     * @brief Ticker for timing animations.
     */
    Ticker ticker;

    /**
     * @brief Keep track of animation phase; swap colors and PWM.
     */
    int phase = 0;

    /**
     * @brief Animate the current state
     */
    void animateState();
};

#endif