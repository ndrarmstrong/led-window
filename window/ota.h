// Over-the-air updates & remote access

#ifndef OTA_H
#define OTA_H

#include "config.h"
#include "env.h"
#include <ArduinoOTA.h>
#include "wifiAssociation.h"

class OTA
{
public:
    // Create an instance of the OTA class
    OTA(WiFiAssociation *wifiAssoc);

    // Run OTA setup functions
    void setup();

    // Run OTA loop functions
    void loop();

    // Whether we are currently associated
    bool isEnabled() { return enabled; }

private:
    // Wi-Fi instance, for checking state
    WiFiAssociation *wifi;

    // Whether OTA is currently enabled
    bool enabled = false;

    // Blink state for ready LED
    bool blinkState = false;

    // Starts OTA
    void startOTA();
};

#endif