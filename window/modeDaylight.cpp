#include "modeDaylight.h"

void ModeDaylight::onStart()
{
    // On start, fade from black
    PanelColor fadeUp = activeColor;
    activeColor = {};
    transitionStep = -1;
    transitionDurationS = DEFAULT_TRANSITION_DURATION_S;
    configure(fadeUp);
}

void ModeDaylight::onStop()
{
    // On stop, skip to the end of the transition if one is in progress
    if (transitionStep >= 0)
    {
        activeColor = nextColor;
        nextColor = {};
        transitionStep = -1;
    }

    ticker.detach();
}

void ModeDaylight::onConfigMessage(byte *payload, unsigned int length)
{
    StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
    bool success = false;

    DeserializationError err = deserializeJson(reqDoc, payload, length);

    if (err == DeserializationError::Ok && reqDoc.containsKey("r") && reqDoc.containsKey("g") && reqDoc.containsKey("b") && reqDoc.containsKey("dw"))
    {
        PanelColor fadeTo = {};
        fadeTo.r = (uint8_t)reqDoc["r"];
        fadeTo.g = (uint8_t)reqDoc["g"];
        fadeTo.b = (uint8_t)reqDoc["b"];
        fadeTo.dw = (uint8_t)reqDoc["dw"];

        configure(fadeTo);
        success = true;
    }
    else
    {
        Log::get().print("System: Unable to parse daylight mode message: ");
        Log::get().println(err.c_str());
    }

    Mqtt::get().acknowledge(Config::MQTT_MSG_TOPIC_DAYLIGHT, success);
}

void ModeDaylight::configure(PanelColor color)
{
    // Reset transition if one is in progress
    if (transitionStep >= 0)
    {
        activeColor = currentColor();
        transitionStep = -1;
    }

    // Requested color is immediately active if this mode is not enabled
    if (!enabled)
    {
        activeColor = color;
        return;
    }

    nextColor = color;

    // Render the first step
    Log::get().print("Daylight: fade from ");
    Log::get().printColor(activeColor);
    Log::get().print(" to ");
    Log::get().printColor(nextColor);
    Log::get().print(" over ");
    Log::get().print(transitionDurationS);
    Log::get().println("s");

    transitionStep = 0;
    render();

    // Next steps happen on timer
    ticker.attach_ms(1000 / FRAMES_PER_SECOND, std::bind(&ModeDaylight::render, this));
}

void ModeDaylight::render()
{
    // Check to see if we're at the end of the fade
    if (transitionStep > FRAMES_PER_SECOND * transitionDurationS)
    {
        transitionStep = -1;
        activeColor = nextColor;
        nextColor = {};
        ticker.detach();
        Leds::get().showColor(activeColor.r, activeColor.g, activeColor.b, activeColor.dw);
        Log::get().println("Daylight: fade ended");
        return;
    }

    PanelColor show = currentColor();
    Leds::get().showColor(show.r, show.g, show.b, show.dw);

    transitionStep++;
}

PanelColor ModeDaylight::currentColor()
{
    if (transitionStep < 0)
    {
        return activeColor;
    }

    int totalSteps = FRAMES_PER_SECOND * transitionDurationS;

    PanelColor currentColor = {};
    currentColor.r = ModeDaylight::channelFraction(activeColor.r, nextColor.r, totalSteps, transitionStep);
    currentColor.g = ModeDaylight::channelFraction(activeColor.g, nextColor.g, totalSteps, transitionStep);
    currentColor.b = ModeDaylight::channelFraction(activeColor.b, nextColor.b, totalSteps, transitionStep);
    currentColor.dw = ModeDaylight::channelFraction(activeColor.dw, nextColor.dw, totalSteps, transitionStep);

    return currentColor;
}

uint8_t ModeDaylight::channelFraction(uint8_t startLevel, uint8_t endLevel, int totalSteps, int currentStep)
{
    // Compute at increased precision before casting down
    int outputLevel = ((totalSteps - currentStep) * (int)startLevel) + (currentStep * (int)endLevel);
    outputLevel = outputLevel / totalSteps;

    return (uint8_t)outputLevel;
}