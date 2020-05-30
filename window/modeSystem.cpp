#include "modeSystem.h"

// TODO:
// Wi-Fi state
// MQTT state
// System temperature [G - - - W - - - - - - R]

ModeSystem::ModeSystem(System *system, Mqtt *mqtt, Leds *leds)
{
    this->system = system;
    this->mqtt = mqtt;
    this->leds = leds;
}

void ModeSystem::start()
{
    if (enabled)
    {
        return;
    }

    frameCounter = 0;
    enabled = true;
    ticker.attach_ms(1000 / FRAMES_PER_SECOND, std::bind(&ModeSystem::animateState, this));
}

void ModeSystem::stop()
{
    if (!enabled)
    {
        return;
    }

    enabled = false;
    ticker.detach();
}

void ModeSystem::animateState()
{
    frameCounter++;

    int wifiStart = 20;
    int mqttStart = 10;
    int otaStart = 0;
    uint8_t wifiHue = 0;
    uint8_t mqttHue = 160;
    uint8_t otaHue = 85;

    // Wi-Fi spinner
    if (!system->isAssociated())
    {
        animateSpinner(wifiStart, wifiHue);
    }
    else
    {
        stopSpinner(wifiStart, wifiHue);
    }

    // MQTT spinner
    if (!system->isAssociated())
    {
        clearSpinner(mqttStart);
    }
    else if (!mqtt->isConnected())
    {
        animateSpinner(mqttStart, mqttHue);
    }
    else
    {
        stopSpinner(mqttStart, mqttHue);
    }

    // OTA spinner
    switch (system->getOtaState())
    {
    case OtaState::Enabled:
        animateSpinner(otaStart, otaHue);
        break;
    case OtaState::InProgress:
        fillSpinner(otaStart, otaHue, system->getOtaProgress());
        break;
    case OtaState::Success:
        stopSpinner(otaStart, otaHue);
        break;
    case OtaState::Error:
        stopSpinner(otaStart, wifiHue);
        break;
    default:
        clearSpinner(otaStart);
    }

    showTemp();

    leds->show();
}

void ModeSystem::animateSpinner(int start, uint8_t hue)
{
    unsigned long animationStep = frameCounter % (36 * 2);
    bool increasing = animationStep < 36;
    unsigned long directionStep = increasing ? animationStep : animationStep - 36;
    unsigned short subpixel = directionStep % 4;
    uint8_t fadeStep = (256 / 5) - 1;

    // Clear spinner
    clearSpinner(start);

    // Quarter pixel rendering, with a 1.5 width pixel.

    // Head pixel fades up from 20% to 80%
    uint8_t headLevel = fadeStep * (subpixel + 1);

    // Body is 100% when head 20%, then fades
    uint8_t bodyLevel = 255 - (subpixel * (fadeStep + 10));

    // Only have a tail when head is 20%
    uint8_t tailLevel = subpixel == 0 ? 25 : 0;

    // Head increments every four
    int headPos = (directionStep / 4) + 1;
    int bodyPos = headPos - 1;
    int tailPos = headPos - 2;

    // Invert if we are decreasing;
    headPos = increasing ? headPos : 9 - headPos;
    bodyPos = increasing ? bodyPos : 9 - bodyPos;
    tailPos = increasing ? tailPos : 9 - tailPos;

    if (headPos >= 0 && headPos < 10)
    {
        leds->topLeds[start + headPos] = CHSV(hue, 255, headLevel);
    }

    if (bodyPos >= 0 && bodyPos < 10)
    {
        leds->topLeds[start + bodyPos] = CHSV(hue, 255, bodyLevel);
    }

    if (tailPos >= 0 && tailPos < 10)
    {
        leds->topLeds[start + tailPos] = CHSV(hue, 255, tailLevel);
    }
}

void ModeSystem::fillSpinner(int start, uint8_t hue, int percent)
{
    int fullLeds = percent / 10;
    uint8_t partialBrightness = (128 / 10) * (percent % 10);

    for (int i = 0; i < 10; i++)
    {
        uint8_t brightness = 0;

        if (i <= fullLeds)
        {
            brightness = 128;
        }
        else if (i == fullLeds + 1)
        {
            brightness = partialBrightness;
        }

        leds->topLeds[start + i] = CHSV(hue, 255, brightness);
    }
}

void ModeSystem::stopSpinner(int start, uint8_t hue)
{
    for (int i = 0; i < 10; i++)
    {
        leds->topLeds[start + i] = CHSV(hue, 255, 128);
    }
}

void ModeSystem::clearSpinner(int start)
{
    for (int i = 0; i < 10; i++)
    {
        leds->topLeds[start + i] = CRGB::Black;
    }
}

void ModeSystem::showTemp()
{
    uint8_t coolHue = 171;
    uint8_t warmHue = 255;
    uint8_t hueStep = 6;
    uint8_t brightStep = 10;
    int startPixel = 20;

    for (int i = 0; i < 10; i++)
    {
        bool flip = i < 5;
        uint8_t hue = flip ? coolHue + hueStep * i : warmHue - hueStep * (9 - i);
        uint8_t brightness = 115 - (flip ? brightStep * i : brightStep * (9 - i));

        leds->bottomLeds[startPixel + i] = CHSV(hue, 255, brightness);
    }

    int lowTemp = 15;
    int tempStep = 3;
    uint8_t currentBrightness = 70;

    if (isnan(system->getTemperature()) || system->getTemperature() <= 0)
    {
        return;
    }

    int currentPixel = (int)((system->getTemperature() - lowTemp) / tempStep);
    currentPixel = std::max(std::min(currentPixel, 9), 0);
    leds->bottomLeds[startPixel + currentPixel] = CRGB(currentBrightness, currentBrightness, currentBrightness);
}