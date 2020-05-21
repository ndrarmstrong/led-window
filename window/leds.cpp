#include "leds.h"

Leds::Leds() {}

void Leds::setup()
{
    // Use a lower PWM freq and resolution to minimize CPU time
    analogWriteFreq(Config::PWM_HZ);
    analogWriteRange(Config::PWM_RANGE);

    // Pin setup
    pinMode(Config::PIN_WHITE_STRIP, OUTPUT);
    pinMode(Config::PIN_TOP_COLOR_DATA, OUTPUT);
    pinMode(Config::PIN_BOTTOM_COLOR_DATA, OUTPUT);

    // Initial pin state
    analogWrite(Config::PIN_WHITE_STRIP, 0);
    digitalWrite(Config::PIN_TOP_COLOR_DATA, LOW);
    digitalWrite(Config::PIN_BOTTOM_COLOR_DATA, LOW);

    // FastLED
    FastLED.addLeds<WS2812, Config::PIN_TOP_COLOR_DATA, EOrder::BRG>(topLeds, topLedsSize());
    FastLED.addLeds<WS2812, Config::PIN_BOTTOM_COLOR_DATA, EOrder::BRG>(bottomLeds, bottomLedsSize());
    reset();
}

void Leds::reset()
{
    clear();
    show();
}

void Leds::clear()
{
    FastLED.clear(true);
    whiteLeds = 0;
}

void Leds::show()
{
    // Disable white, render FastLED, then re-enable white
    disableWhiteLeds();
    pwmTicker.detach();
    pwmTicker.once_ms(1, std::bind(&Leds::delayedShow, this));
}

void Leds::disableWhiteLeds()
{
    analogWrite(Config::PIN_WHITE_STRIP, 0);
}

void Leds::delayedShow()
{
    FastLED.show();
    pwmTicker.once_ms(fastledWriteTimeMs, std::bind(&Leds::enableWhiteLeds, this));
}

void Leds::enableWhiteLeds()
{
    analogWrite(Config::PIN_WHITE_STRIP, whiteLeds);
}

void Leds::showColor(uint8_t r, uint8_t g, uint8_t b, int dw)
{
    CRGB color = CRGB();
    color.r = r;
    color.g = g;
    color.b = b;
    fill_solid(topLeds, topLedsSize(), color);
    fill_solid(bottomLeds, bottomLedsSize(), color);
    whiteLeds = dw;

    // TODO power control

    show();
}

void Leds::showColorStrip(uint8_t r, uint8_t g, uint8_t b, CRGB *leds, int ledsSize)
{
    clear();
    CRGB color = CRGB();
    color.r = r;
    color.g = g;
    color.b = b;
    fill_solid(leds, ledsSize, color);
    show();
}

void Leds::showTop(uint8_t r, uint8_t g, uint8_t b)
{
    showColorStrip(r, g, b, topLeds, topLedsSize());
}

void Leds::showBottom(uint8_t r, uint8_t g, uint8_t b)
{
    showColorStrip(r, g, b, bottomLeds, bottomLedsSize());
}

void Leds::showWhite(int dw)
{
    clear();
    whiteLeds = dw;
    show();
}

void Leds::showLeds(uint8_t r, uint8_t g, uint8_t b, CRGB *leds, int from, int to)
{
    clear();
    CRGB color = CRGB();
    color.r = r;
    color.g = g;
    color.b = b;
    for (int i = from; i <= to; i++)
    {
        leds[i] = color;
    }
    show();
}