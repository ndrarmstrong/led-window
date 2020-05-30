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
     * @brief Construct a new Mqtt object
     * @param sys System instance
     */
    Mqtt(System *sys);

    /**
     * @brief Run MQTT loop functions
     */
    void loop();

    /**
     * @brief Whether we are connected to MQTT broker
     */
    bool isConnected() { return mqttClient.connected(); }

private:
    /**
     * @brief System instance.
     */
    System *sys;

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