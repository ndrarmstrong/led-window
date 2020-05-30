#ifndef MQTT_H
#define MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include "system.h"
#include "config.h"

/**
 * @brief Manages service connection, over MQTT
 */
class Mqtt
{
public:
    /**
     * @brief Returns a reference to the global mqtt connection.
     * @return The global mqtt connection.
     */
    static Mqtt &get();

    /**
     * @brief Run MQTT loop functions
     */
    void loop();

    /**
     * @brief Whether we are connected to MQTT broker
     */
    bool isConnected() { return mqttClient.connected(); }

    /**
     * @brief Publish a message to a topic
     * 
     * @param topic Topic to publish to, not including device prefix
     * @param message Message to publish
     */
    void publish(const char *topic, const char *message);

private:
    /**
     * @brief Private constructor - only one instance
     */
    Mqtt();

    /**
     * @brief ESP Wi-Fi client instance.
     */
    WiFiClient espWifiClient;

    /**
     * @brief MQTT client instance.
     */
    PubSubClient mqttClient;

    /**
     * @brief MQTT receive buffer
     */
    char msgBuffer[Config::MQTT_MESSAGE_SIZE_B];

    /**
     * @brief Ticker for connecting to MQTT
     */
    Ticker connectTicker;

    /**
     * @brief Connect/reconnect to MQTT broker
     */
    void connect();
};

#endif