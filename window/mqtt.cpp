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
    }
    else
    {
        Serial.println("MQTT: Failed to connect to broker");
    }
}

void Mqtt::publish(const char *topic, const char *message)
{
    String deviceTopic = String("device/");
    deviceTopic += getDeviceId();
    deviceTopic += "/";
    deviceTopic += topic;

    if (mqttClient.connected())
    {
        mqttClient.publish(deviceTopic.c_str(), message);
    }
}