#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <Arduino.h>
#include <StreamString.h>
#include "mqtt.h"

/**
 * @brief Stream implementation that logs to MQTT
 */
class MqttLogStream : public StreamString
{
public:
    /**
     * @brief Write the buffered content to MQTT
     */
    void flush() override;
};

#endif