// Wi-Fi subsystem

#include "config.h"
#include "env.h"
#include <Arduino.h>
#include<ESP8266WiFi.h>

class WiFiAssociation {
    public:
        // Create an instance of the Wi-Fi class
        WiFiAssociation();
        
        // Run Wi-Fi setup functions
        void setup();

        // Run Wi-Fi loop functions
        void loop();

        // Whether we are currently associated
        bool isAssociated() { return isAssocated; }

    private:
        // Whether we are currently associated
        bool isAssocated = false;
};