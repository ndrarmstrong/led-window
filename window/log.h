#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>
#include "WString.h"
#include "Printable.h"
#include <inttypes.h>
#include "Print.h"
#include "stdlib_noniso.h"
#include "mqtt.h"
#include "mqttLogStream.h"
#include "panelColor.h"

class Log
{
public:
    /**
     * @brief Returns a reference to the global log manager.
     * @return The global log manager
     */
    static Log &get();

    // Print functions (from Print.h)
    size_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));
    size_t print(const __FlashStringHelper *);
    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    size_t print(const Printable &);

    // Println functions (from Print.h)
    size_t println(const __FlashStringHelper *);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(const Printable &);
    size_t println(void);

    // Utility functions

    /**
     * @brief Print color to log
     * @param color Color to print
     */
    void printColor(PanelColor color);

    /**
     * @brief Print color to log
     * @param r Red level
     * @param g Green level
     * @param b Blue level
     * @param dw Daylight white level
     */
    void printColor(uint8_t r, uint8_t g, uint8_t b, int dw);

private:
    /**
     * @brief Private constructor - only one instance
     */
    Log();

    /**
     * @brief MQTT log stream
     */
    MqttLogStream logStream;
};

#endif