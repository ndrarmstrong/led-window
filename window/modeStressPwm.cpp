#include "modeStressPwm.h"

void ModeStressPwm::onStart()
{
    frameCounter = 0;
    ticker.attach_ms(1000 / FRAMES_PER_SECOND, std::bind(&ModeStressPwm::animateState, this));
}

void ModeStressPwm::onStop()
{
    ticker.detach();
}

void ModeStressPwm::animateState()
{
    static const int maxFrames = 2 * (FADE_FRAMES + HOLD_FRAMES);
    static const int stripStart = 30;
    static const int stripEnd = stripStart + 10;
    static const int fadeStep = 256 / FADE_FRAMES;

    frameCounter++;

    if (frameCounter >= maxFrames)
    {
        Log::get().print("Stress PWM: Increment phase: ");
        Log::get().print(phase);
        phase = (phase + 1) % 4;
        Log::get().print(" -> ");
        Log::get().println(phase);
    }

    frameCounter = frameCounter % maxFrames;

    // Fade up -> hold up -> fade down -> hold down
    bool up = frameCounter < (FADE_FRAMES + HOLD_FRAMES);
    bool hold = up ? frameCounter > FADE_FRAMES : frameCounter > (maxFrames - HOLD_FRAMES - 1);

    // Only update if not holding
    if (!hold)
    {
        uint8_t fadeLevel = up ? frameCounter * fadeStep : 256 - fadeStep * (frameCounter - FADE_FRAMES - HOLD_FRAMES + 1);
        CRGB testColor = phase < 2 ? CRGB::Red : CRGB::Blue;

        for (int i = stripStart; i < stripEnd; i++)
        {
            Leds::get().topLeds[i] = testColor;
            Leds::get().topLeds[i].nscale8_video(fadeLevel);

            Leds::get().bottomLeds[i] = testColor;
            Leds::get().bottomLeds[i].nscale8_video(fadeLevel);
        }
    }

    Leds::get().whiteLeds = (phase % 2) == 0 ? 0 : 1;
    Leds::get().show();
}