#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include "logStream.h"
#include "mqtt.h"

class Log
{
public:
    /**
     * @brief Returns a reference to the global log manager.
     * @return The global log manager
     */
    static Log &get();

    /**
     * @brief Print log text
     */
    void print(const char c[]);

    /**
     * @brief Print log line
     */
    void println(const char c[]);

private:
    /**
     * @brief Private constructor - only one instance
     */
    Log();

    /**
     * @brief MQTT log stream
     */
    LogStream logStream;
};

#endif