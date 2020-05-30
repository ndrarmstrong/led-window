#include "log.h"

Log::Log() {}

Log &Log::get()
{
    static Log *global = 0;

    if (!global)
    {
        global = new Log();
    }

    return *global;
}

void Log::print(const char c[])
{
    Serial.print(c);
    if (Mqtt::get().isConnected())
    {
        logStream.print(c);
    }
}

void Log::println(const char c[])
{
    Serial.println(c);
    if (Mqtt::get().isConnected())
    {
        logStream.println(c);
        logStream.flush();
    }
}