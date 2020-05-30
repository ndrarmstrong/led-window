#include "mqtt.h"

Mqtt::Mqtt(System *sys)
{
    this->sys = sys;
    mqttClient.setClient(espWifiClient);
    mqttClient.setBufferSize(Config::MQTT_MESSAGE_SIZE_B);
    mqttClient.setKeepAlive(Config::MQTT_KEEPALIVE_S);
    mqttClient.setServer(Config::MQTT_BROKER_ADDRESS, Config::MQTT_BROKER_PORT);
    mqttClient.setSocketTimeout(Config::MQTT_BROKER_CONNECT_TIMEOUT_S);
}

void Mqtt::loop()
{
    if (mqttClient.connected())
    {
        mqttClient.loop();
        return;
    }

    int reconnectDelayMs = (Config::MQTT_BROKER_CONNECT_TIMEOUT_S + 1) * 1000;

    if (this->sys->isAssociated() && !connectTicker.active())
    {
        // Attach reconnect ticker (and signal we're connecting)
        connectTicker.attach_ms_scheduled(reconnectDelayMs, std::bind(&Mqtt::connect, this));

        // Connect now
        connect();
    }
}

void Mqtt::connect()
{
    Serial.print("MQTT: Connect to broker ");
    Serial.print(Config::MQTT_BROKER_ADDRESS);
    Serial.print(":");
    Serial.println(Config::MQTT_BROKER_PORT);

    // This is (unfortunately) blocking, but usually happens quickly
    if (mqttClient.connect(this->sys->getDeviceId().c_str(), "device", Config::MQTT_BROKER_SECRET))
    {
        Serial.println("MQTT: Connected to broker");
        connectTicker.detach();
    }
    else
    {
        Serial.println("MQTT: Failed to connect to broker");
    }
}