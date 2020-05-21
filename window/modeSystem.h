#ifndef MODESYSTEM_H
#define MODESYSTEM_H

#include <Arduino.h>
#include <Ticker.h>
#include "system.h"
#include "leds.h"

/**
 * @brief System mode; used to display status via LEDs
 * 
 */
class ModeSystem
{
public:
    /**
     * @brief Construct a new Mode System object
     * 
     * @param system System instance
     * @param leds Leds instance
     */
    ModeSystem(System *system, Leds *leds);

    /**
     * @brief Start running system mode.
     */
    void start();

    /**
     * @brief Stop running system mode.
     */
    void stop();

private:
    static const int FRAMES_PER_SECOND = 24;

    /**
     * @brief Whether this module is enabled.
     */
    bool enabled = false;

    /**
     * @brief System instance.
     */
    System *system;

    /**
     * @brief Leds instance.
     */
    Leds *leds;

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
     * @brief Animate the current system state
     */
    void animateState();

    /**
     * @brief Animate a state spinner.
     * @param start Start LED
     * @param hue Spinner hue
     */
    void animateSpinner(int start, uint8_t hue);

    /**
     * @brief Fill a state spinner to a certain percentage
     * @param start Start LED
     * @param hue Spinner hue
     * @param percent Fill percentage
     */
    void fillSpinner(int start, uint8_t hue, int percent);

    /**
     * @brief Stop a state spinner, indicating completion.
     * @param start Start LED
     * @param hue Spinner hue
     */
    void stopSpinner(int start, uint8_t hue);

    /**
     * @brief Clear a state spinner.
     * @param start Start LED
     */
    void clearSpinner(int start);

    /**
     * @brief Show the current system temperature.
     */
    void showTemp();
};

#endif