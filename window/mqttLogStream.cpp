#include "logStream.h"

void MqttLogStream::flush()
{
    if (Mqtt::get().isConnected())
    {
        replace(String("\""), String("'"));
        replace(String("\n"), String(""));
        replace(String("\r"), String(""));

        String badJson = String("{\"line\":\"");
        badJson += c_str();
        badJson += "\"}";

        Mqtt::get().publish("log", badJson.c_str());
    }
    clear();
}