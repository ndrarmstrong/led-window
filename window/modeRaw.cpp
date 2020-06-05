#include "modeRaw.h"

void ModeRaw::reset()
{
    color = {};
}

void ModeRaw::onConfigMessage(byte *payload, unsigned int length)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
    bool success = false;

    DeserializationError err = deserializeJson(reqDoc, payload, length);

    if (err == DeserializationError::Ok && reqDoc.containsKey("r") && reqDoc.containsKey("g") && reqDoc.containsKey("b") && reqDoc.containsKey("dw"))
    {
        configure((uint8_t)reqDoc["r"], (uint8_t)reqDoc["g"], (uint8_t)reqDoc["b"], (uint8_t)reqDoc["dw"]);
        success = true;
    }
    else
    {
        Log::get().print("System: Unable to parse raw mode message: ");
        Log::get().println(err.c_str());
    }

    Mqtt::get().acknowledge(Config::MQTT_MSG_TOPIC_RAW, success);
}

void ModeRaw::configure(uint8_t r, uint8_t g, uint8_t b, int dw)
{
    Log::get().print("Raw mode: configure color (");
    Log::get().print(r);
    Log::get().print(",");
    Log::get().print(g);
    Log::get().print(",");
    Log::get().print(b);
    Log::get().print(",");
    Log::get().print(dw);
    Log::get().println(")");

    color.r = r;
    color.g = g;
    color.b = b;
    color.dw = dw;

    render();
}

void ModeRaw::render()
{
    if (!enabled)
    {
        return;
    }

    Leds::get().showColor(color.r, color.g, color.b, color.dw);
}