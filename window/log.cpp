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

size_t Log::printf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    size_t len = Serial.printf(format, arg);
    if (Mqtt::get().isConnected())
    {
        logStream.printf(format, arg);
    }
    va_end(arg);
    return len;
}

size_t Log::print(const __FlashStringHelper *ifsh)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(ifsh);
    }
    return Serial.print(ifsh);
}

size_t Log::print(const String &s)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(s);
    }
    return Serial.print(s);
}

size_t Log::print(const char str[])
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(str);
    }
    return Serial.print(str);
}

size_t Log::print(char c)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(c);
    }
    return Serial.print(c);
}

size_t Log::print(unsigned char b, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(b, base);
    }
    return Serial.print(b, base);
}

size_t Log::print(int n, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(n, base);
    }
    return Serial.print(n, base);
}

size_t Log::print(unsigned int n, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(n, base);
    }
    return Serial.print(n, base);
}

size_t Log::print(long n, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(n, base);
    }
    return Serial.print(n, base);
}

size_t Log::print(unsigned long n, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(n, base);
    }
    return Serial.print(n, base);
}

size_t Log::print(double n, int digits)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(n, digits);
    }
    return Serial.print(n, digits);
}

size_t Log::println(const __FlashStringHelper *ifsh)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(ifsh);
        logStream.flush();
    }
    return Serial.print(ifsh);
}

size_t Log::print(const Printable &x)
{
    if (Mqtt::get().isConnected())
    {
        logStream.print(x);
        logStream.flush();
    }
    return Serial.print(x);
}

size_t Log::println(void)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println();
        logStream.flush();
    }
    return Serial.println();
}

size_t Log::println(const String &s)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(s);
        logStream.flush();
    }
    return Serial.println(s);
}

size_t Log::println(const char c[])
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(c);
        logStream.flush();
    }
    return Serial.println(c);
}

size_t Log::println(char c)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(c);
        logStream.flush();
    }
    return Serial.println(c);
}

size_t Log::println(unsigned char b, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(b, base);
        logStream.flush();
    }
    return Serial.println(b, base);
}

size_t Log::println(int num, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(num, base);
        logStream.flush();
    }
    return Serial.println(num, base);
}

size_t Log::println(unsigned int num, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(num, base);
        logStream.flush();
    }
    return Serial.println(num, base);
}

size_t Log::println(long num, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(num, base);
        logStream.flush();
    }
    return Serial.println(num, base);
}

size_t Log::println(unsigned long num, int base)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(num, base);
        logStream.flush();
    }
    return Serial.println(num, base);
}

size_t Log::println(double num, int digits)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(num, digits);
        logStream.flush();
    }
    return Serial.println(num, digits);
}

size_t Log::println(const Printable &x)
{
    if (Mqtt::get().isConnected())
    {
        logStream.println(x);
        logStream.flush();
    }
    return Serial.println(x);
}

void Log::printColor(PanelColor color)
{
    printColor(color.r, color.g, color.b, color.dw);
}

void Log::printColor(uint8_t r, uint8_t g, uint8_t b, int dw)
{
    print("(");
    print(r);
    print(",");
    print(g);
    print(",");
    print(b);
    print(",");
    print(dw);
    print(")");
}