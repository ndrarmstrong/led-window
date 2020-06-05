#ifndef MODESYSTEM_H
#define MODESYSTEM_H

#include <Arduino.h>
#include <Ticker.h>
#include "system.h"
#include "mqtt.h"
#include "leds.h"
#include "mode.h"

/**
 * @brief System mode; used to display status via LEDs
 * 
 */
class ModeSystem : public Mode
{
public:
    /**
     * @brief System mode cannot be configured
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
     * @brief How many frames per second the spinners render at.
     */
    static const int FRAMES_PER_SECOND = 24;

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