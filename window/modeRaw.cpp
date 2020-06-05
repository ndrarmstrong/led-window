#include "modeRaw.h"

void ModeRaw::reset()
{
    r = 0;
    g = 0;
    b = 0;
    dw = 0;
}

void ModeRaw::onConfigMessage(byte *payload, unsigned int length)
{
    // TODO
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

    this->r = r;
    this->g = g;
    this->b = b;
    this->dw = dw;
    render();
}

void ModeRaw::render()
{
    if (!enabled)
    {
        return;
    }

    Leds::get().showColor(r, g, b, dw);
}