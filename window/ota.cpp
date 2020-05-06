#include "ota.h"

OTA::OTA(WiFiAssociation *wifiAssoc)
{
    wifi = wifiAssoc;
}

void OTA::setup()
{
    // No setup - needs to happen after Wi-Fi associates
}

void OTA::loop()
{
    if (!wifi->isAssociated())
    {
        // Can't do OTA when no Wi-Fi
        return;
    }

    // Enable for debug
    if (!enabled && OTA_ENABLED && OTA_DEBUG)
    {
        startOTA();
    }

    // TODO Check for request to enable OT for non-debug

    if (enabled)
    {
        ArduinoOTA.handle();
    }
}

void OTA::startOTA()
{
    if (!enabled)
    {
        ArduinoOTA.setHostname(HOSTNAME);
        ArduinoOTA.setPassword(OTA_PASSWORD);

        ArduinoOTA.onStart([]() {
            Serial.println("OTA Started");
            // TODO disable LEDs
        });

        ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
            Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
            digitalWrite(PIN_READY_LED, blinkState ? HIGH : LOW);
            blinkState = !blinkState;
        });

        ArduinoOTA.onEnd([]() {
            Serial.println("OTA Complete");
            // TODO Should I restart here?
        });

        ArduinoOTA.onError([](ota_error_t error) {
            ESP.restart();
        });

        ArduinoOTA.begin();

        Serial.println("OTA Enabled");
        enabled = true;
    }
}
