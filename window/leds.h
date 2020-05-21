
// LED control module
// Wraps FastLED and PWM

#ifndef LEDS_H
#define LEDS_H

#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include "config.h"
#include <FastLED.h>
#include <Ticker.h>

/**
 * The type of power limits that can be applied.
 */
enum PowerLimitMode
{
    /**
     * @brief No power limit; render color as specified regardless of power consumption.
     */
    NoLimit,
    /**
     * @brief Apply a power limit to avoid overloading power supply;
     * when insufficient power, prefer brightness over color accuracy.
     */
    PreferBrightness,
    /**
     * @brief Apply a power limit to avoid overloading power supply;
     * when insufficient power, prefer color accuracy over brightness.
     */
    PreferColor
};

/**
 * LED control module; wraps FastLED and PWM.
 */
class Leds
{
public:
    /**
     * @brief Create an instance of the LEDs class
     */
    Leds();

    /**
     * @brief Initial setup of LEDs and output pins.
     */
    void setup();

    /**
     * @brief Reset LEDs to default values, and turn off
     */
    void reset();

    /**
     * @brief Clear LEDs state to default, without showing
     */
    void clear();

    /**
     * @brief Render current state to LEDs
     */
    void show();

    /**
     * Show the specified color.
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     * @param dw Daylight white level
     */
    void showColor(uint8_t r, uint8_t g, uint8_t b, int dw);

    /**
     * Show the specified color on the top LEDs.
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     */
    void showTop(uint8_t r, uint8_t g, uint8_t b);

    /**
     * Show the specified color on the top LEDs.
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     */
    void showBottom(uint8_t r, uint8_t g, uint8_t b);

    /**
     * Show the specified level on the white LEDs
     * @param dw Daylight white level
     */
    void showWhite(int dw);

    /**
     * @brief Show the specified color on the specified LEDs.
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     * @param leds Target LED strip
     * @param from Min LED
     * @param to Max LED
     */
    void showLeds(uint8_t r, uint8_t g, uint8_t b, CRGB *leds, int from, int to);

    /**
     * @brief Type of power limit to apply when rendering
     */
    PowerLimitMode limitPower = PowerLimitMode::PreferBrightness;

    /**
     * @brief Top color LED array.
     * Prefer showColor() over accessing LEDs directly.
     */
    CRGB topLeds[Config::TOP_COLOR_LED_COUNT];

    /**
     * @brief Bottom color LED arary.
     * Prefer showColor() over accessing LEDs directly.
     */
    CRGB bottomLeds[Config::BOTTOM_COLOR_LED_COUNT];

    /**
     * @brief White LED state.
     * Prefer showColor() over accessing LEDs directly.
     */
    int whiteLeds = 0;

    /**
     * @brief Number of color LEDs on the top of the window.
     * @return int The number of color LEDs on the top of the window.
     */
    int topLedsSize() { return Config::TOP_COLOR_LED_COUNT; }

    /**
     * @brief Number of color LEDs on the bottom of the window.
     * @return int The number of color LEDs on the bottom of the window.
     */
    int bottomLedsSize() { return Config::BOTTOM_COLOR_LED_COUNT; }

private:
    /**
     * @brief Approximately how long it takes FastLED to write all LEDs, in us
     */
    static constexpr const int fastledWriteTimeUs = 30 * (Config::TOP_COLOR_LED_COUNT + Config::BOTTOM_COLOR_LED_COUNT);

    /**
     * @brief Approximately how long it takes FastLED to write all LEDs, in ms
     */
    static constexpr const int fastledWriteTimeMs = (fastledWriteTimeUs / 1000);

    /**
     * @brief Ticker for resuming PWM;
     */
    Ticker pwmTicker;

    /**
     * @brief Disable white LEDs; used when FastLED is writing.
     * NB: This does a fairly good job "anecdotally" of correcting
     * color LED rendering issues.
     */
    void disableWhiteLeds();

    /**
     * @brief Call FastLED.show, then enableWhiteLeds.  As a function
     * so we can use it on a timer.
     * NB: This does a fairly good job "anecdotally" of correcting
     * color LED rendering issues.
     */
    void delayedShow();

    /**
     * @brief Enable white LEDs; used when FastLED is done writing.
     * NB: This does a fairly good job "anecdotally" of correcting
     * color LED rendering issues.
     */
    void enableWhiteLeds();

    /**
     * @brief Show the specified color on the target strip.
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     * @param leds Target LED strip
     * @param ledsSize Size of target LED strip
     */
    void showColorStrip(uint8_t r, uint8_t g, uint8_t b, CRGB *leds, int ledsSize);
};

#endif