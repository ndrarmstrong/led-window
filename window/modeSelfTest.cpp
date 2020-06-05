#include "modeSelfTest.h"

void ModeSelfTest::onStart()
{
    phase = 0;
    ticker.attach_ms_scheduled(750, std::bind(&ModeSelfTest::render, this));
}

void ModeSelfTest::onStop()
{
    ticker.detach();
}

void ModeSelfTest::render()
{
    switch (phase)
    {
    case 0:
        Leds::get().showTop(255, 0, 0);
        break;
    case 1:
        Leds::get().showTop(0, 255, 0);
        break;
    case 2:
        Leds::get().showTop(0, 0, 255);
        break;
    case 3:
        Leds::get().showBottom(255, 0, 0);
        break;
    case 4:
        Leds::get().showBottom(0, 255, 0);
        break;
    case 5:
        Leds::get().showBottom(0, 0, 255);
        break;
    case 6:
        Leds::get().showWhite(3);
        break;
    case 7:
        Leds::get().showWhite(10);
        break;
    case 8:
        Leds::get().showWhite(40);
        break;
    case 9:
        Leds::get().showColor(150, 0, 0, 0);
        break;
    case 10:
        Leds::get().showColor(0, 150, 0, 0);
        break;
    case 11:
        Leds::get().showColor(0, 0, 150, 0);
        break;
    case 12:
        Leds::get().showLeds(0, 0, 255, Leds::get().topLeds, 30, 39);
        break;
    case 13:
        Leds::get().showLeds(0, 0, 255, Leds::get().topLeds, 20, 29);
        break;
    case 14:
        Leds::get().showLeds(0, 0, 255, Leds::get().topLeds, 10, 19);
        break;
    case 15:
        Leds::get().showLeds(0, 0, 255, Leds::get().topLeds, 0, 9);
        break;
    case 16:
        Leds::get().showLeds(0, 0, 255, Leds::get().bottomLeds, 0, 9);
        break;
    case 17:
        Leds::get().showLeds(0, 0, 255, Leds::get().bottomLeds, 10, 19);
        break;
    case 18:
        Leds::get().showLeds(0, 0, 255, Leds::get().bottomLeds, 20, 29);
        break;
    case 19:
        Leds::get().showLeds(0, 0, 255, Leds::get().bottomLeds, 30, 39);
        break;
    }

    phase = (phase + 1) % 20;
}