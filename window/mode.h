#ifndef MODE_H
#define MODE_H

#include <Arduino.h>
#include "leds.h"
#include "log.h"

class Mode
{
public:
    /**
     * @brief Start mode
     * 
     */
    void start();

    /**
     * @brief Stop mode
     * 
     */
    void stop();

    /**
     * @brief Handle incoming message to configure mode
     * @param payload Message payload
     * @param length Message length
     */
    virtual void onConfigMessage(byte *payload, unsigned int length);

protected:
    /**
     * @brief Whether this module is enabled
     */
    bool enabled = false;

    /**
     * @brief Called when mode starts
     * 
     */
    virtual void onStart(){};

    /**
     * @brief Called when mode stops
     * 
     */
    virtual void onStop(){};
};

#endif