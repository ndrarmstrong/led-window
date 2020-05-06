#include "wifiAssociation.h"

WiFiAssociation::WiFiAssociation()
{
    // Empty constructor
}

void WiFiAssociation::setup()
{
    Serial.print("Wi-Fi start: Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.hostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_CREDENTIAL);
}

void WiFiAssociation::loop()
{
    if (WiFi.status() == WL_CONNECTED && !associated)
    {
        // We are now connected to Wi-Fi
        Serial.print("Wi-Fi connected: ");
        Serial.print(WIFI_SSID);
        Serial.print(", IP: ");
        Serial.println(WiFi.localIP());
        associated = true;
    }
    else if (WiFi.status() != WL_CONNECTED)
    {
        associated = false;
    }
}