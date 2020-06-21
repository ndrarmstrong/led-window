#include "lightSensor.h"

LightSensor::LightSensor()
{
}

LightSensor &LightSensor::get()
{
    static LightSensor *global = 0;

    if (!global)
    {
        global = new LightSensor();
    }

    return *global;
}

void LightSensor::start()
{
    if (enabled)
    {
        return;
    }

    enabled = true;
    reconfigure();
}

void LightSensor::stop()
{
    if (!enabled)
    {
        return;
    }

    enabled = false;
    reconfigure();
}

void LightSensor::reconfigure()
{
    publishTicker.detach();

    // Nothing more to do if we are disabled
    if (!enabled)
    {
        Log::get().println("Sensor: Disabling");
        return;
    }

    Log::get().print("Sensor: Configuring - ");
    Log::get().print(publishIntervalS);
    Log::get().print("s, lux ");
    Log::get().print(luxGain);
    Log::get().print(" ");
    Log::get().print(luxIntegrationMs);
    Log::get().print(", rgb ");
    Log::get().print(rgbIntensityRange);
    Log::get().print(" ");
    Log::get().print(rgbIrFilterOffset);
    Log::get().print(" ");
    Log::get().println(rgbIrFilterLevel);

    Wire.begin();

    // Lux Sensor
    if (!luxSensor->begin())
    {
        Log::get().println("Sensor: Failed to initialize lux sensor");
        stop();
        return;
    }

    luxSensor->setGain(luxGain);
    luxSensor->setIntegTime(luxIntegrationMs);

    // RGB Sensor
    if (!rgbSensor.init())
    {
        Log::get().println("Sensor: Failed to initialize RGB sensor");
        stop();
        return;
    }

    rgbSensor.config(CFG1_MODE_RGB | rgbIntensityRange, rgbIrFilterOffset | rgbIrFilterLevel, CFG_DEFAULT);

    publishTicker.attach_ms_scheduled(publishIntervalS * 1000, std::bind(&LightSensor::readAndPublish, this));
    readAndPublish();
}

void LightSensor::delayedReconfigure()
{
    reconfigureTicker.once_ms(250, std::bind(&LightSensor::reconfigure, this));
}

void LightSensor::readAndPublish()
{
    readSensors();
    publishLightLevel();

    uint16_t maxVal = 65535;

    Log::get().print("Sensor: (");
    Log::get().print(rgbRedLevel, HEX);
    Log::get().print(",");
    Log::get().print(rgbGreenLevel, HEX);
    Log::get().print(",");
    Log::get().print(rgbBlueLevel, HEX);
    Log::get().print(")/(");
    Log::get().print((int)(1000 * (rgbRedLevel / (float)maxVal)) / (float)1000);
    Log::get().print(",");
    Log::get().print((int)(1000 * (rgbGreenLevel / (float)maxVal)) / (float)1000);
    Log::get().print(",");
    Log::get().print((int)(1000 * (rgbBlueLevel / (float)maxVal)) / (float)1000);
    Log::get().print(") at ");
    Log::get().print(luxLevel);
    Log::get().println(" lux");
}

void LightSensor::readSensors()
{
    luxLevel = luxSensor->readLight();
    luxWhiteLevel = luxSensor->readWhiteLight();
    rgbRedLevel = rgbSensor.readRed();
    rgbGreenLevel = rgbSensor.readGreen();
    rgbBlueLevel = rgbSensor.readBlue();
}

void LightSensor::publishLightLevel()
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    unsigned char resBuf[Config::MQTT_MESSAGE_SIZE_B];

    resDoc["lux"] = luxLevel;
    resDoc["luxWhite"] = luxWhiteLevel;
    resDoc["red"] = rgbRedLevel;
    resDoc["green"] = rgbGreenLevel;
    resDoc["blue"] = rgbBlueLevel;

    size_t resLen = serializeJson(resDoc, resBuf);
    Mqtt::get().publish(Config::MQTT_MSG_TOPIC_LIGHT_LEVEL, resBuf, resLen);
}

void LightSensor::onLightLevelMessage(byte *payload, unsigned int length)
{
    // Light level requests have no body
    readAndPublish();

    Log::get().print("Sensor: Responded to ");
    Log::get().println(Config::MQTT_MSG_TOPIC_LIGHT_LEVEL);
}

void LightSensor::publishLux()
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    unsigned char resBuf[Config::MQTT_MESSAGE_SIZE_B];

    resDoc["gain"] = luxGain;
    resDoc["integrationTimeMs"] = luxIntegrationMs;

    size_t resLen = serializeJson(resDoc, resBuf);
    Mqtt::get().publish(Config::MQTT_MSG_TOPIC_LUX, resBuf, resLen);
}

void LightSensor::publishRgb()
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    unsigned char resBuf[Config::MQTT_MESSAGE_SIZE_B];

    resDoc["intensityRange"] = rgbIntensityRange;
    resDoc["irFilterOffset"] = rgbIrFilterOffset;
    resDoc["irFilterLevel"] = rgbIrFilterLevel;

    size_t resLen = serializeJson(resDoc, resBuf);
    Mqtt::get().publish(Config::MQTT_MSG_TOPIC_RGB, resBuf, resLen);
}

void LightSensor::publishInterval()
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    unsigned char resBuf[Config::MQTT_MESSAGE_SIZE_B];

    resDoc["intervalS"] = publishIntervalS;

    size_t resLen = serializeJson(resDoc, resBuf);
    Mqtt::get().publish(Config::MQTT_MSG_TOPIC_INTERVAL, resBuf, resLen);
}

void LightSensor::onIntervalMessage(byte *payload, unsigned int length)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
    bool success = false;

    DeserializationError err = deserializeJson(reqDoc, payload, length);

    if (err == DeserializationError::Ok)
    {
        if (reqDoc.containsKey("intervalS"))
        {
            publishIntervalS = (int)reqDoc["intervalS"];

            if (publishIntervalS < 2)
            {
                publishIntervalS = 2;
            }

            delayedReconfigure();
        }

        success = true;
    }
    else
    {
        Log::get().print("System: Unable to parse interval message: ");
        Log::get().println(err.c_str());
    }

    publishInterval();
    Log::get().print("Sensor: Responded to ");
    Log::get().println(Config::MQTT_MSG_TOPIC_INTERVAL);
}

void LightSensor::onLuxMessage(byte *payload, unsigned int length)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
    bool success = false;
    bool readOnly = true;

    DeserializationError err = deserializeJson(reqDoc, payload, length);

    if (err == DeserializationError::Ok)
    {
        if (reqDoc.containsKey("gain"))
        {
            luxGain = (float)reqDoc["gain"];
            readOnly = false;
        }

        if (reqDoc.containsKey("integrationTimeMs"))
        {
            luxIntegrationMs = (int)reqDoc["integrationTimeMs"];
            readOnly = false;
        }

        if (!readOnly)
        {
            delayedReconfigure();
        }

        success = true;
    }
    else
    {
        Log::get().print("System: Unable to parse lux message: ");
        Log::get().println(err.c_str());
    }

    publishLux();
    Log::get().print("Sensor: Responded to ");
    Log::get().println(Config::MQTT_MSG_TOPIC_LUX);
}

void LightSensor::onRgbMessage(byte *payload, unsigned int length)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
    bool success = false;
    bool readOnly = true;

    DeserializationError err = deserializeJson(reqDoc, payload, length);

    if (err == DeserializationError::Ok)
    {
        if (reqDoc.containsKey("intensityRange"))
        {
            rgbIntensityRange = (int)reqDoc["intensityRange"];
            readOnly = false;
        }

        if (reqDoc.containsKey("irFilterOffset"))
        {
            rgbIrFilterOffset = (int)reqDoc["irFilterOffset"];
            readOnly = false;
        }

        if (reqDoc.containsKey("irFilterLevel"))
        {
            rgbIrFilterLevel = (int)reqDoc["irFilterLevel"];
            readOnly = false;
        }

        if (!readOnly)
        {
            delayedReconfigure();
        }

        success = true;
    }
    else
    {
        Log::get().print("System: Unable to parse rgb message: ");
        Log::get().println(err.c_str());
    }

    publishRgb();
    Log::get().print("Sensor: Responded to ");
    Log::get().println(Config::MQTT_MSG_TOPIC_RGB);
}