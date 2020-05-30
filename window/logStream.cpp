#include "logStream.h"

void LogStream::flush()
{
    if (Mqtt::get().isConnected())
    {
        String badJson = String("{\"line\":\"");
        badJson += c_str();
        badJson += "\"}";
        badJson.replace(String("\""), String("'"));
        badJson.replace(String("\n"), String(""));
        badJson.replace(String("\r"), String(""));
        Mqtt::get().publish("log", badJson.c_str());
    }
    clear();
}