#include "system.h"

System::System()
{
    dht = new DHT_Unified(Config::PIN_TEMP_SENSOR, DHT11);
}

System &System::get()
{
    static System *global = 0;

    if (!global)
    {
        global = new System();
    }

    return *global;
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
    Log::get().print("Wi-Fi start: Connecting to ");
    Log::get().println(Config::WIFI_SSID);

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
        Log::get().print("Wi-Fi connected: ");
        Log::get().print(Config::WIFI_SSID);
        Log::get().print(", IP: ");
        Log::get().println(WiFi.localIP());
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
            Log::get().println("OTA Started");
        });

        ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
            otaProgress = (progress / (total / 100));
            Log::get().print("OTA Progress: ");
            Log::get().print(otaProgress);
            Log::get().println("%");
            digitalWrite(Config::PIN_READY_LED, blinkState ? HIGH : LOW);
            blinkState = !blinkState;
        });

        ArduinoOTA.onEnd([this]() {
            if (otaState != OtaState::Error)
            {
                otaState = OtaState::Success;
            }
            Log::get().println("OTA Complete");
        });

        ArduinoOTA.onError([this](ota_error_t error) {
            otaState = OtaState::Error;
            Log::get().printf("Error[%u]: ", error);

            if (error == OTA_AUTH_ERROR)
            {
                Log::get().println("Auth Failed");
            }
            else if (error == OTA_BEGIN_ERROR)
            {
                Log::get().println("Begin Failed");
            }
            else if (error == OTA_CONNECT_ERROR)
            {
                Log::get().println("Connect Failed");
            }
            else if (error == OTA_RECEIVE_ERROR)
            {
                Log::get().println("Receive Failed");
            }
            else if (error == OTA_END_ERROR)
            {
                Log::get().println("End Failed");
            }
        });

        ArduinoOTA.begin();

        Log::get().println("OTA Enabled");
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

    Log::get().print("System: ");
    Log::get().print((int)temperature);
    Log::get().print(F("°C, "));
    Log::get().print((int)humidity);
    Log::get().println("% humidity");
}

void System::onSysMessage(byte *payload, unsigned int length)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
    bool success = false;

    DeserializationError err = deserializeJson(reqDoc, payload, length);

    if (err == DeserializationError::Ok)
    {
        if (reqDoc.containsKey("otaEnabled") && (boolean)reqDoc["otaEnabled"] == true)
        {
            startOTA();
        }
        success = true;
    }
    else
    {
        Log::get().print("Unable to parse sys message: ");
        Log::get().println(err.c_str());
    }

    Mqtt::get().acknowledge(Config::MQTT_MSG_TOPIC_SYS, success);
}

void System::onDescribeMessage(byte *payload, unsigned int length)
{
    // Describe requests have no body
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    char resBuf[Config::MQTT_MESSAGE_SIZE_B];

    resDoc["device"] = String(ESP.getChipId(), HEX);
    resDoc["freeHeapB"] = ESP.getFreeHeap();
    resDoc["sketchSizeB"] = ESP.getSketchSize();
    resDoc["freeSketchSpaceB"] = ESP.getFreeSketchSpace();
    resDoc["otaEnabled"] = otaState != OtaState::Disabled;
    resDoc["temperatureC"] = temperature;
    resDoc["relativeHumidityPct"] = humidity;

    size_t resLen = serializeJson(resDoc, resBuf);
    Mqtt::get().publish(Config::MQTT_MSG_TOPIC_DESCRIBE, resBuf, resLen);

    Log::get().print("Responded to ");
    Log::get().println(Config::MQTT_MSG_TOPIC_DESCRIBE);
}