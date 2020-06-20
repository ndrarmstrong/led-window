#include "system.h"

System::System()
{
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
    setupWiFi();
}

void System::setupWiFi()
{
    Log::get().print("System: Wi-Fi start; connecting to ");
    Log::get().println(Config::WIFI_SSID);

    wifiAssociatingBlinkPhase = 0;
    wifiAssociatingTicker.attach_ms_scheduled(200, std::bind(&System::blinkWifiAssociating, this));
    blinkWifiAssociating();

    WiFi.hostname(Config::HOSTNAME);
    WiFi.begin(Config::WIFI_SSID, Config::WIFI_CREDENTIAL);
}

void System::loop()
{
    updateWatchdog();
    checkWiFiAssociated();

    if (otaState != OtaState::Disabled)
    {
        ArduinoOTA.handle();
    }
}

void System::updateWatchdog()
{
    if (!associated || !Mqtt::get().isConnected())
    {
        if (!watchdogTicker.active())
        {
            Log::get().println("System: Not connected to Wi-Fi/MQTT; starting watchdog");
            watchdogTicker.once_ms(Config::SYSTEM_WATCHDOG_INTERVAL_MS, std::bind(&System::resetSystem, this));
        }
    }
    else if (watchdogTicker.active())
    {
        Log::get().println("System: Connected to Wi-Fi/MQTT; stopping watchdog");
        watchdogTicker.detach();
    }
}

void System::resetSystem()
{
    Log::get().println("System: Resetting due to watchdog or MQTT command.");
    ESP.reset();
}

void System::checkWiFiAssociated()
{
    if (WiFi.status() == WL_CONNECTED && !associated)
    {
        // We are now connected to Wi-Fi
        Log::get().print("System: Wi-Fi connected: ");
        Log::get().print(Config::WIFI_SSID);
        Log::get().print(", IP: ");
        Log::get().println(WiFi.localIP());
        associated = true;

        wifiAssociatingTicker.detach();
        digitalWrite(Config::PIN_READY_LED, HIGH);
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
            Log::get().println("System: OTA Started");
        });

        ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
            otaProgress = (progress / (total / 100));
            Log::get().print("System: OTA Progress: ");
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
            Log::get().println("System: OTA Complete");
        });

        ArduinoOTA.onError([this](ota_error_t error) {
            otaState = OtaState::Error;

            if (error == OTA_AUTH_ERROR)
            {
                Log::get().println("System: OTA Auth Failed");
            }
            else if (error == OTA_BEGIN_ERROR)
            {
                Log::get().println("System: OTA Begin Failed");
            }
            else if (error == OTA_CONNECT_ERROR)
            {
                Log::get().println("System: OTA Connect Failed");
            }
            else if (error == OTA_RECEIVE_ERROR)
            {
                Log::get().println("System: OTA Receive Failed");
            }
            else if (error == OTA_END_ERROR)
            {
                Log::get().println("System: OTA End Failed");
            }
        });

        ArduinoOTA.begin();

        Log::get().println("System: OTA Enabled");
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

        if (reqDoc.containsKey("reset") && (boolean)reqDoc["reset"] == true)
        {
            // Reset after short delay
            resetTicker.once_ms(100, std::bind(&System::resetSystem, this));
        }

        success = true;
    }
    else
    {
        Log::get().print("System: Unable to parse sys message: ");
        Log::get().println(err.c_str());
    }

    Mqtt::get().acknowledge(Config::MQTT_MSG_TOPIC_SYS, success);
}

void System::onDescribeMessage(byte *payload, unsigned int length)
{
    // Describe requests have no body
    publishDescribe();

    Log::get().print("System: Responded to ");
    Log::get().println(Config::MQTT_MSG_TOPIC_DESCRIBE);
}

void System::publishDescribe()
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    char resBuf[Config::MQTT_MESSAGE_SIZE_B];

    resDoc["device"] = String(ESP.getChipId(), HEX);
    resDoc["freeHeapB"] = ESP.getFreeHeap();
    resDoc["sketchSizeB"] = ESP.getSketchSize();
    resDoc["freeSketchSpaceB"] = ESP.getFreeSketchSpace();
    resDoc["otaEnabled"] = otaState != OtaState::Disabled;

    size_t resLen = serializeJson(resDoc, resBuf);
    Mqtt::get().publish(Config::MQTT_MSG_TOPIC_DESCRIBE, resBuf, resLen);
}