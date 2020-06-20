#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <ArduinoJson.h>
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
     * @brief Set the callback function for received messages
     * @param callback Callback function for received messages
     */
    void setCallback(MQTT_CALLBACK_SIGNATURE);

    /**
     * @brief Run MQTT loop functions
     * @param isAssociated Whether Wi-Fi is associated (to avoid a circular dependency with system)
     */
    void loop(bool isAssociated);

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

    /**
     * @brief Publish a message to a topic
     * 
     * @param topic Topic to publish to, not including device prefix
     * @param message Message to publish
     * @param length Length of message
     */
    void publish(const char *topic, const char *payload, unsigned int plength);

    /**
     * @brief Publish an acknowledgement response to a topic.
     * 
     * @param topic Topic to publish to.
     * @param success Whether the request was successful
     */
    void acknowledge(const char *topic, bool success);

    /**
     * @brief Get the full device topic from the function topic
     * 
     * @param topic Function topic
     * @return Full device topic
     */
    String deviceTopic(const char *topic) { return deviceTopic(String(topic)); }

    /**
     * @brief Get the full device topic from the function topic
     * 
     * @param topic Function topic
     * @return Full device topic
     */
    String deviceTopic(String topic);

    /**
     * @brief Get the full device request topic from the function topic
     * 
     * @param topic Function topic
     * @return Full device request topic
     */
    String deviceReqTopic(const char *topic) { return deviceReqTopic(String(topic)); }

    /**
     * @brief Get the full device request topic from the function topic
     * 
     * @param topic Function topic
     * @return Full device request topic
     */
    String deviceReqTopic(String topic);

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

    /**
     * @brief Get the device ID as a hex string
     * @return String The device ID as a hex string
     */
    String getDeviceId() { return String(ESP.getChipId(), HEX); }

    /**
     * @brief Set up subscriptions for all modules.
     */
    void subscribe();
};

#endif