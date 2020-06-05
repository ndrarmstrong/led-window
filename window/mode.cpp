#include "mode.h"

void Mode::start()
{
    if (enabled)
    {
        return;
    }

    enabled = true;
    onStart();
}

void Mode::stop()
{
    if (!enabled)
    {
        return;
    }

    enabled = false;
    onStop();
}