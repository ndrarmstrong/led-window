#include "mqtt.h"

Mqtt::Mqtt()
{
    mqttClient.setClient(espWifiClient);
    mqttClient.setBufferSize(Config::MQTT_MESSAGE_SIZE_B);
    mqttClient.setKeepAlive(Config::MQTT_KEEPALIVE_S);
    mqttClient.setServer(Config::MQTT_BROKER_ADDRESS, Config::MQTT_BROKER_PORT);
    mqttClient.setSocketTimeout(Config::MQTT_BROKER_CONNECT_TIMEOUT_S);
}

Mqtt &Mqtt::get()
{
    static Mqtt *global = 0;

    if (!global)
    {
        global = new Mqtt();
    }

    return *global;
}

void Mqtt::loop(bool isAssociated)
{
    if (mqttClient.connected())
    {
        mqttClient.loop();
        return;
    }

    int reconnectDelayMs = (Config::MQTT_BROKER_CONNECT_TIMEOUT_S + 1) * 1000;

    if (isAssociated && !connectTicker.active())
    {
        // Attach reconnect ticker (and signal we're connecting)
        connectTicker.attach_ms_scheduled(reconnectDelayMs, std::bind(&Mqtt::connect, this));

        // Connect now
        connect();
    }
}

void Mqtt::connect()
{
    // Don't use Log, because we're A) not connected and B) would create a circular dependency
    Serial.print("MQTT: Connect to broker ");
    Serial.print(Config::MQTT_BROKER_ADDRESS);
    Serial.print(":");
    Serial.println(Config::MQTT_BROKER_PORT);

    // This is (unfortunately) blocking, but usually happens quickly
    if (mqttClient.connect(getDeviceId().c_str(), "device", Config::MQTT_BROKER_SECRET))
    {
        Serial.println("MQTT: Connected to broker");
        connectTicker.detach();
        subscribe();
    }
    else
    {
        Serial.println("MQTT: Failed to connect to broker");
    }
}

void Mqtt::publish(const char *topic, const char *message)
{
    if (mqttClient.connected())
    {
        mqttClient.publish(deviceTopic(topic).c_str(), message, false);
    }
}

void Mqtt::publish(const char *topic, const unsigned char *payload, unsigned int plength)
{
    if (mqttClient.connected())
    {
        mqttClient.publish(deviceTopic(topic).c_str(), payload, plength, false);
    }
}

void Mqtt::acknowledge(const char *topic, bool success)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> resDoc;
    unsigned char resBuf[Config::MQTT_MESSAGE_SIZE_B];
    resDoc["result"] = success ? 0 : 1;
    size_t resLen = serializeJson(resDoc, resBuf);

    publish(topic, resBuf, resLen);

    Serial.print("MQTT: Acknowledged ");
    Serial.println(topic);
}

void Mqtt::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    mqttClient.setCallback(callback);
}

String Mqtt::deviceTopic(String topic)
{
    String devTopic = String(Config::MQTT_TOPIC_NAMESPACE);
    devTopic += "/device/";
    devTopic += getDeviceId();
    devTopic += "/";
    devTopic += topic;
    return devTopic;
}

String Mqtt::deviceReqTopic(String topic)
{
    String devTopic = deviceTopic(topic);
    devTopic += Config::MQTT_TOPIC_REQ_SUFFIX;
    return devTopic;
}

void Mqtt::subscribe()
{
    mqttClient.subscribe(deviceReqTopic(Config::MQTT_MSG_TOPIC_DESCRIBE).c_str());
    mqttClient.subscribe(deviceReqTopic(Config::MQTT_MSG_TOPIC_SYS).c_str());
    mqttClient.subscribe(deviceReqTopic(Config::MQTT_MSG_TOPIC_DAYLIGHT).c_str());
    mqttClient.subscribe(deviceReqTopic(Config::MQTT_MSG_TOPIC_RAW).c_str());
    mqttClient.subscribe(deviceReqTopic(Config::MQTT_MSG_TOPIC_MODE).c_str());
}