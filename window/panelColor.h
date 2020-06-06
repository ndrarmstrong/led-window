#ifndef PANELCOLOR_H
#define PANELCOLOR_H

#include <Arduino.h>

/**
 * @brief Full-panel color
 */
struct PanelColor
{
    /**
     * @brief Red channel level.
     */
    uint8_t r = 0;

    /**
     * @brief Green channel level.
     */
    uint8_t g = 0;

    /**
     * @brief Blue channel level.
     */
    uint8_t b = 0;

    /**
     * @brief Daylight white channel level.
     */
    uint8_t dw = 0;
};

#endif