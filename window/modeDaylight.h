#ifndef MODEDAYLIGHT_H
#define MODEDAYLIGHT_H

#include <Arduino.h>
#include <Ticker.h>
#include "leds.h"
#include "log.h"
#include "mode.h"
#include "panelColor.h"

/**
 * @brief Daylight mode; render daylight value with smooth transitions
 * 
 */
class ModeDaylight : public Mode
{
public:
    /**
     * @brief Configure this mode
     * @param color Color to show
     */
    void configure(PanelColor color);

    /**
     * @brief Handle incoming message to configure mode
     * @param payload Message payload
     * @param length Message length
     */
    void onConfigMessage(byte *payload, unsigned int length) override;

protected:
    /**
     * @brief Runs on mode start
     */
    void onStart() override;

    /**
     * @brief Runs on mode stop
     */
    void onStop() override;

private:
    /**
     * @brief How many frames per second the transition renders at.
     */
    static const int FRAMES_PER_SECOND = 24;

    /**
     * @brief How many seconds to transition over, by default.
     */
    static const int DEFAULT_TRANSITION_DURATION_S = 5;

    /**
     * @brief Compute a channel level based on progress through transition
     * @param startLevel Channel start level
     * @param endLevel Channel end level
     * @param totalSteps Total number of steps
     * @param currentStep Current step
     * @return uint8_t Channel level based on progress
     */
    static uint8_t channelFraction(uint8_t startLevel, uint8_t endLevel, int totalSteps, int currentStep);

    /**
     * @brief Active color
     */
    PanelColor activeColor = {};

    /**
     * @brief Color being transitioned to
     */
    PanelColor nextColor = {};

    /**
     * @brief Step in transition; negative if no transition in progress.
     */
    int transitionStep = 0;

    /**
     * @brief Duration of the current transition.
     */
    int transitionDurationS = DEFAULT_TRANSITION_DURATION_S;

    /**
     * @brief Ticker for timing animations.
     */
    Ticker ticker;

    /**
     * @brief Render active value to the LED strips
     */
    void render();

    /**
     * @brief Compute the current color, partway through a transition
     * @return PanelColor Current color
     */
    PanelColor currentColor();
};

#endif