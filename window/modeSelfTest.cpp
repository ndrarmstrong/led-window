#include "modeSelfTest.h"

ModeSelfTest::ModeSelfTest(Leds *leds)
{
    this->leds = leds;
}

void ModeSelfTest::start()
{
    if (enabled)
    {
        return;
    }

    enabled = true;
    phase = 0;
    ticker.attach_ms_scheduled(750, std::bind(&ModeSelfTest::render, this));
}

void ModeSelfTest::stop()
{
    if (!enabled)
    {
        return;
    }

    enabled = false;
    ticker.detach();
}

void ModeSelfTest::render()
{
    switch (phase)
    {
    case 0:
        leds->showTop(255, 0, 0);
        break;
    case 1:
        leds->showTop(0, 255, 0);
        break;
    case 2:
        leds->showTop(0, 0, 255);
        break;
    case 3:
        leds->showBottom(255, 0, 0);
        break;
    case 4:
        leds->showBottom(0, 255, 0);
        break;
    case 5:
        leds->showBottom(0, 0, 255);
        break;
    case 6:
        leds->showWhite(3);
        break;
    case 7:
        leds->showWhite(10);
        break;
    case 8:
        leds->showWhite(40);
        break;
    case 9:
        leds->showColor(150, 0, 0, 0);
        break;
    case 10:
        leds->showColor(0, 150, 0, 0);
        break;
    case 11:
        leds->showColor(0, 0, 150, 0);
        break;
    case 12:
        leds->showLeds(0, 0, 255, leds->topLeds, 30, 39);
        break;
    case 13:
        leds->showLeds(0, 0, 255, leds->topLeds, 20, 29);
        break;
    case 14:
        leds->showLeds(0, 0, 255, leds->topLeds, 10, 19);
        break;
    case 15:
        leds->showLeds(0, 0, 255, leds->topLeds, 0, 9);
        break;
    case 16:
        leds->showLeds(0, 0, 255, leds->bottomLeds, 0, 9);
        break;
    case 17:
        leds->showLeds(0, 0, 255, leds->bottomLeds, 10, 19);
        break;
    case 18:
        leds->showLeds(0, 0, 255, leds->bottomLeds, 20, 29);
        break;
    case 19:
        leds->showLeds(0, 0, 255, leds->bottomLeds, 30, 39);
        break;
    }

    phase = (phase + 1) % 20;
}