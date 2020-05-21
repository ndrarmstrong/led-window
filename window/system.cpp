#include "system.h"

System::System(Leds *leds)
{
    this->leds = leds;
    dht = new DHT_Unified(Config::PIN_TEMP_SENSOR, DHT11);
}

void System::setup()
{
    // TODO: Double-blink when connecting to Wi-Fi,
    // solid when Wi-Fi connected
    pinMode(Config::PIN_READY_LED, OUTPUT);

    // Start monitoring system temperature
    // The first read can be wrong, so read twice to begin.
    dht->begin();
    temperatureReadTicker.once_ms_scheduled(800, std::bind(&System::updateTemp, this));

    setupWiFi();
}

void System::setupWiFi()
{
    Serial.print("Wi-Fi start: Connecting to ");
    Serial.println(Config::WIFI_SSID);

    wifiAssociatingBlinkPhase = 0;
    wifiAssociatingTicker.attach_ms_scheduled(200, std::bind(&System::blinkWifiAssociating, this));
    blinkWifiAssociating();

    WiFi.hostname(Config::HOSTNAME);
    WiFi.begin(Config::WIFI_SSID, Config::WIFI_CREDENTIAL);
}

void System::loop()
{
    checkWiFiAssociated();

    if (otaState != OtaState::Disabled)
    {
        ArduinoOTA.handle();
    }
}

void System::checkWiFiAssociated()
{
    if (WiFi.status() == WL_CONNECTED && !associated)
    {
        // We are now connected to Wi-Fi
        Serial.print("Wi-Fi connected: ");
        Serial.print(Config::WIFI_SSID);
        Serial.print(", IP: ");
        Serial.println(WiFi.localIP());
        associated = true;

        wifiAssociatingTicker.detach();
        digitalWrite(Config::PIN_READY_LED, HIGH);

        // Temporarily enable OTA
        // startOTA();
    }
    else if (WiFi.status() != WL_CONNECTED)
    {
        associated = false;
        otaState = OtaState::Disabled;
    }
}

void System::startOTA()
{
    if (otaState == OtaState::Disabled)
    {
        ArduinoOTA.setHostname(Config::HOSTNAME);
        ArduinoOTA.setPassword(Config::SYSTEM_OTA_PASSWORD);

        ArduinoOTA.onStart([this]() {
            otaState = OtaState::InProgress;
            otaProgress = 0;
            Serial.println("OTA Started");
        });

        ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
            otaProgress = (progress / (total / 100));
            Serial.printf("OTA Progress: %u%%\r", otaProgress);
            digitalWrite(Config::PIN_READY_LED, blinkState ? HIGH : LOW);
            blinkState = !blinkState;
        });

        ArduinoOTA.onEnd([this]() {
            if (otaState != OtaState::Error)
            {
                otaState = OtaState::Success;
            }
            Serial.println("OTA Complete");
        });

        ArduinoOTA.onError([this](ota_error_t error) {
            otaState = OtaState::Error;
            Serial.printf("Error[%u]: ", error);

            if (error == OTA_AUTH_ERROR)
            {
                Serial.println("Auth Failed");
            }
            else if (error == OTA_BEGIN_ERROR)
            {
                Serial.println("Begin Failed");
            }
            else if (error == OTA_CONNECT_ERROR)
            {
                Serial.println("Connect Failed");
            }
            else if (error == OTA_RECEIVE_ERROR)
            {
                Serial.println("Receive Failed");
            }
            else if (error == OTA_END_ERROR)
            {
                Serial.println("End Failed");
            }
        });

        ArduinoOTA.begin();

        Serial.println("OTA Enabled");
        otaState = OtaState::Enabled;
    }
}

void System::blinkWifiAssociating()
{
    if (wifiAssociatingBlinkPhase == 0 || wifiAssociatingBlinkPhase == 2)
    {
        digitalWrite(Config::PIN_READY_LED, HIGH);
    }
    else
    {
        digitalWrite(Config::PIN_READY_LED, LOW);
    }

    wifiAssociatingBlinkPhase = (wifiAssociatingBlinkPhase + 1) % 8;
}

void System::updateTemp()
{
    sensors_event_t tempEvent;
    sensors_event_t humidityEvent;
    dht->temperature().getEvent(&tempEvent);
    dht->humidity().getEvent(&humidityEvent);

    temperature = tempEvent.temperature;
    humidity = humidityEvent.relative_humidity;

    // Reattach timer; this allows us to read quickly to start,
    // then slow down
    temperatureReadTicker.once_ms_scheduled(30000, std::bind(&System::updateTemp, this));

    Serial.print("System: ");
    Serial.print((int)temperature);
    Serial.print(F("°C, "));
    Serial.print((int)humidity);
    Serial.println("% humidity");
}
