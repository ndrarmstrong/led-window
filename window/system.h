#ifndef SYSTEM_H
#define SYSTEM_H

#include "config.h"
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
#include "log.h"

/**
 * @brief State of OTA updates
 */
enum OtaState
{
    /**
     * @brief OTA updates are disabled
     * 
     */
    Disabled,

    /**
     * @brief OTA updates are enabled, but not in progres.
     * 
     */
    Enabled,

    /**
     * @brief OTA updates are enabled and an OTA update is in progress.
     */
    InProgress,

    /**
     * @brief An OTA update has completed successfully.
     */
    Success,

    /**
     * @brief An OTA update has stopped with an error.
     * 
     */
    Error
};

/**
 * @brief System module - OTA, state, Wi-Fi
 * 
 */
class System
{
public:
    /**
     * @brief Returns a reference to the global system manager.
     * @return The global system manager
     */
    static System &get();

    /**
     * @brief Run system setup functions
     */
    void setup();

    /**
     * @brief Run system loop functions
     */
    void loop();

    /**
     * @brief Get the current OTA state
     * @return The current OTA state
     */
    OtaState getOtaState() { return otaState; }

    /**
     * @brief Get the current OTA progress
     * @return int The current OTA progress
     */
    int getOtaProgress() { return otaProgress; }

    /**
     * @brief Whether we are currently associated to Wi-Fi
     * 
     * @return true Associated to Wi-Fi
     * @return false Not associated to Wi-Fi
     */
    bool isAssociated() { return associated; }

    /**
     * @brief Get the current system temperature
     * @return float Current system temperature
     */
    float getTemperature() { return temperature; }

    /**
     * @brief Get the current system humidity
     * @return float Current system humidity
     */
    float getHumidity() { return humidity; }

    /**
     * @brief Handle incoming sys messages to set system state
     * @param payload Message payload
     * @param length Message length
     */
    void onSysMessage(byte *payload, unsigned int length);

    /**
     * @brief Handle incoming describe messages requesting system info
     * @param payload Message payload
     * @param length Message length
     */
    void onDescribeMessage(byte *payload, unsigned int length);

private:
    /**
     * @brief Private constructor - only one instance.
     */
    System();

    /**
     * @brief The state of OTA updates.
     */
    OtaState otaState = OtaState::Disabled;

    /**
     * @brief The progress of the current OTA update, if one is running.
     */
    int otaProgress = 0;

    /**
     * @brief Blink state for system LED
     */
    bool blinkState = false;

    /**
     * @brief Whether we are currently associated to Wi-Fi
     */
    bool associated = false;

    /**
     * @brief Current system temperature.
     * 
     */
    float temperature;

    /**
     * @brief Current system humidity.
     * 
     */
    float humidity;

    /**
     * @brief Ticker for blinking when Wi-Fi associating
     */
    Ticker wifiAssociatingTicker;

    /**
     * @brief Watchdog ticker; resets the unit if it is unconnected from Wi-Fi or MQTT for too long.
     */
    Ticker watchdogTicker;

    /**
     * @brief Reset ticker; used to delay to allow MQTT responses to go out.
     */
    Ticker resetTicker;

    /**
     * @brief Counter for Wi-Fi associating blink code
     */
    int wifiAssociatingBlinkPhase = 0;

    /**
     * @brief DHT temperature sensor
     */
    DHT_Unified *dht;

    /**
     * @brief Read temperature periodically.
     */
    Ticker temperatureReadTicker;

    /**
     * @brief Starts OTA
     */
    void startOTA();

    /**
     * @brief Setup Wi-Fi association.
     * 
     */
    void setupWiFi();

    /**
     * @brief Update watchdog state.
     */
    void updateWatchdog();

    /**
     * @brief Reset the system.
     */
    void resetSystem();

    /**
     * @brief Check to see if Wi-Fi is now associated
     * 
     */
    void checkWiFiAssociated();

    /**
     * @brief Blink when Wi-Fi associating
     */
    void blinkWifiAssociating();

    /**
     * @brief Update the current system temperature.
     */
    void updateTemp();

    /**
     * @brief Publish a describe message.  This can be in response to a request, or as periodic status broadcast.
     */
    void publishDescribe();
};

#endif