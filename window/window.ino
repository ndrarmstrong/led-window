#include <Arduino.h>
#include "config.h"
#include "wifiAssociation.h"
#include "ota.h"

// FastLED library
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// Program config
#define CHANNEL_TEST_WHITE_FACTOR 3   // Keep the current draw under 1A - low speed fan!
#define WHITE_POWER_TEST_STEP_SIZE 1  // Speed of the power test
#define WHITE_POWER_TEST_MAX_DUTY 240 // Limit power to <5A
#define FULL_PANEL_WHITE_LEVEL 180    // Limit power to < 5A
#define MODE 0                        // 0 = Self test        \
                                      // 1 = White power test \
                                      // 2 = Full panel test  \
                                      // 3 = Color temperature
#define MODE_0_TICK_MS 500
#define MODE_1_TICK_MS 20
#define MODE_2_TICK_MS 5000
#define MODE_3_TICK_MS 3000

// Globals
WiFiAssociation wifi;
OTA ota(&wifi);
CRGB topLeds[TOP_COLOR_LED_COUNT];
CRGB bottomLeds[BOTTOM_COLOR_LED_COUNT];
int channelIndex = 0;
int tick = 0;
unsigned long lastLoop = 0;
bool decrease = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup");
  wifi.setup();
  ota.setup();

  // Use a lower PWM freq and resolution to minimize CPU time
  analogWriteFreq(PWM_HZ);
  analogWriteRange(PWM_RANGE);

  // Pin setup
  pinMode(PIN_READY_LED, OUTPUT);
  pinMode(PIN_WHITE_STRIP, OUTPUT);
  pinMode(PIN_TOP_COLOR_DATA, OUTPUT);
  pinMode(PIN_BOTTOM_COLOR_DATA, OUTPUT);

  // Initial pin state
  digitalWrite(PIN_READY_LED, HIGH);
  analogWrite(PIN_WHITE_STRIP, 0);
  digitalWrite(PIN_TOP_COLOR_DATA, LOW);
  digitalWrite(PIN_BOTTOM_COLOR_DATA, LOW);

  // FastLED
  FastLED.addLeds<WS2812, PIN_TOP_COLOR_DATA, EOrder::BRG>(topLeds, TOP_COLOR_LED_COUNT);
  FastLED.addLeds<WS2812, PIN_BOTTOM_COLOR_DATA, EOrder::BRG>(bottomLeds, BOTTOM_COLOR_LED_COUNT);
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  wifi.loop();
  ota.loop();

  unsigned long now = millis();

  // Rollover counters
  if (lastLoop > now)
  {
    lastLoop = 0;
  }

  int tickDelay;
  switch (MODE)
  {
  case 0:
    tickDelay = MODE_0_TICK_MS;
    break;
  case 1:
    tickDelay = MODE_1_TICK_MS;
    break;
  case 2:
    tickDelay = MODE_2_TICK_MS;
    break;
  case 3:
    tickDelay = MODE_3_TICK_MS;
    break;
  default:
    tickDelay = 1000;
    break;
  }

  if (now - lastLoop < tickDelay)
  {
    return;
  }

  lastLoop = now;

  switch (MODE)
  {
  case 0:
    selfTest();
    break;
  case 1:
    whitePowerTest();
    break;
  case 2:
    constantTest();
    break;
  case 3:
    colorTempTest();
    break;
  default:
    selfTest();
  }
}

void selfTest()
{
  // Channel swap
  if (tick > 2)
  {
    analogWrite(PIN_WHITE_STRIP, 0);
    FastLED.clear();
    FastLED.show();
    channelIndex = (channelIndex + 1) % 3;
    tick = 0;
  }

  // White
  if (channelIndex == 0)
  {
    int duty = (tick + 1) * (tick + 1) * CHANNEL_TEST_WHITE_FACTOR;
    analogWrite(PIN_WHITE_STRIP, duty);
  }
  else
  {

    // Color
    CRGB color = CRGB::Blue;

    switch (tick)
    {
    case 1:
      color = CRGB::Red;
      break;
    case 2:
      color = CRGB::Green;
      break;
    default:
      break;
    }

    int ledCount = (channelIndex == 1 ? TOP_COLOR_LED_COUNT : BOTTOM_COLOR_LED_COUNT);
    CRGB *leds = (channelIndex == 1 ? topLeds : bottomLeds);

    for (int i = 0; i < ledCount; i++)
    {
      leds[i] = color;
    }

    FastLED.show();
  }

  tick++;
}

void whitePowerTest()
{
  int steps = WHITE_POWER_TEST_MAX_DUTY / WHITE_POWER_TEST_STEP_SIZE;
  int holdTicks = 100;
  int maxTicks = steps + holdTicks;

  if (tick > maxTicks)
  {
    decrease = true;
  }
  else if (tick < 0)
  {
    tick = 0;
    decrease = false;
  }
  else if (tick < steps)
  {
    analogWrite(PIN_WHITE_STRIP, tick * WHITE_POWER_TEST_STEP_SIZE);
  }

  tick = decrease ? tick - 1 : tick + 1;
}

void constantTest()
{
  if (tick > 2)
  {
    tick = 0;
  }

  // Alternate colors
  CRGB color = CRGB::Blue;

  switch (tick)
  {
  case 1:
    color = CRGB::Red;
    break;
  case 2:
    color = CRGB::Green;
    break;
  default:
    break;
  }

  writeAllLeds(color);

  FastLED.show();

  int whiteLevel = 1;
  analogWrite(PIN_WHITE_STRIP, FULL_PANEL_WHITE_LEVEL);

  tick++;
}

void colorTempTest()
{
  int whiteLevel = 0;
  int colorLevel = 0;
  CRGB color = CRGB::Black;

  int temps[] = {2800, 3300, 4000, 5000, 6000, 6400, 8000, 9500};

  if (tick > 7)
  {
    tick = 0;
  }

  int targetTemp = temps[tick];

  switch (targetTemp)
  {
  case 9500:
    whiteLevel = 180;
    colorLevel = 250;
    color = CRGB::Blue;
    break;
  case 8000:
    whiteLevel = 210;
    colorLevel = 155;
    color = CRGB::Blue;
    break;
  case 6400:
    whiteLevel = 210;
    colorLevel = 0;
    color = CRGB::Blue;
    break;
  case 6000:
    whiteLevel = 210;
    colorLevel = 100;
    //color = CRGB::Red;
    color = CRGB(100, 50, 0);
    break;
  case 5500:
    whiteLevel = 180;
    colorLevel = 255;
    //color = CRGB::Red;
    color = CRGB(255, 128, 0);
    break;
  case 5000:
    whiteLevel = 85;
    colorLevel = 255;
    //color = CRGB::Red;
    color = CRGB(255, 128, 0);
    break;
  case 4000:
    whiteLevel = 29;
    colorLevel = 255;
    //color = CRGB::Red;
    color = CRGB(255, 128, 0);
    break;
  case 3300:
    whiteLevel = 15;
    colorLevel = 255;
    //color = CRGB::Red;
    color = CRGB(255, 96, 0);
    break;
  case 2800:
    whiteLevel = 8;
    colorLevel = 255;
    //color = CRGB::Red;
    color = CRGB(255, 64, 0);
    break;
  default:
    whiteLevel = 0;
    colorLevel = 0;
  }

  FastLED.setBrightness(colorLevel);
  writeAllLeds(color);
  FastLED.show();
  analogWrite(PIN_WHITE_STRIP, whiteLevel);
  tick++;
}

void writeAllLeds(CRGB color)
{
  for (int i = 0; i < TOP_COLOR_LED_COUNT; i++)
  {
    topLeds[i] = color;
  }
  for (int j = 0; j < BOTTOM_COLOR_LED_COUNT; j++)
  {
    bottomLeds[j] = color;
  }
}