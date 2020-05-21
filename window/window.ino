#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include "config.h"
#include "system.h"
#include "leds.h"
#include "modeSystem.h"
#include "modeSelfTest.h"
#include "modeRaw.h"

/**
 * @brief LED window display modes.
 */
enum Modes
{
  /**
   * @brief Startup mode - equivalent to system followed by primary mode
   * 
   */
  STARTUP = 0,

  /**
   * @brief System mode - displays system status
   */
  SYSTEM = 1,

  /**
   * @brief Self test mode - shows LED strip channels
   */
  SELF_TEST = 2,

  /**
   * @brief Raw mode - renders color exactly as given
   */
  RAW = 3
};

/**
 * @brief Primary operating mode; activates after Wi-Fi connects.
 */
const Modes primaryMode = Modes::SELF_TEST;

/**
 * @brief Current operating mode.
 */
Modes currentMode = Modes::STARTUP;

/**
 * @brief Leds instance.
 */
Leds leds;

/**
 * @brief System instance.
 */
System sys(&leds);

/**
 * @brief System mode instance.
 */
ModeSystem modeSystem(&sys, &leds);

/**
 * @brief Self test mode instance.
 */
ModeSelfTest modeSelfTest(&leds);

/**
 * @brief Raw mode instance.
 */
ModeRaw modeRaw(&leds);

/**
 * @brief Setup function - runs once on start
 */
void setup()
{
  Serial.begin(115200);
  Serial.println("Setup");
  sys.setup();
  leds.setup();
  modeSystem.start();
}

/**
 * @brief Main microprocessor loop.\
 */
void loop()
{
  sys.loop();

  if (currentMode != Modes::SYSTEM && sys.isAssociated() && sys.getOtaState() != OtaState::Disabled)
  {
    // If OTA is enabled, force into system mode
    switchModes(Modes::SYSTEM);
  }
  else if (currentMode == Modes::STARTUP && sys.isAssociated())
  {
    // If we've completed startup, switch into the primary mode
    switchModes(primaryMode);
  }
  else if (!sys.isAssociated() && currentMode != Modes::STARTUP)
  {
    // We've lost Wi-Fi; startup mode until reconnected
    switchModes(Modes::STARTUP);
  }
}

/**
 * @brief Switch to a new operating mode
 * @param nextMode The new operating mode to switch to
 */
void switchModes(Modes nextMode)
{
  if (currentMode == nextMode)
  {
    return;
  }

  Serial.print("Switching from mode ");
  Serial.print(currentMode);
  Serial.print(" to mode ");
  Serial.println(nextMode);

  switch (currentMode)
  {
  case STARTUP:
  case SYSTEM:
    modeSystem.stop();
    break;
  case SELF_TEST:
    modeSelfTest.stop();
    break;
  case RAW:
    modeRaw.stop();
    break;
  }

  switch (nextMode)
  {
  case STARTUP:
  case SYSTEM:
    modeSystem.start();
    break;
  case SELF_TEST:
    modeSelfTest.start();
    break;
  case RAW:
    modeRaw.start();
    break;
  }

  currentMode = nextMode;
}

// TODO - VERY rough color temperature conversions. Varying intensities.
//   case 9500:
//     whiteLevel = 180;
//     colorLevel = 250;
//     color = CRGB::Blue;
//     break;
//   case 8000:
//     whiteLevel = 210;
//     colorLevel = 155;
//     color = CRGB::Blue;
//     break;
//   case 6400:
//     whiteLevel = 210;
//     colorLevel = 0;
//     color = CRGB::Blue;
//     break;
//   case 6000:
//     whiteLevel = 210;
//     colorLevel = 100;
//     color = CRGB(100, 50, 0);
//     break;
//   case 5500:
//     whiteLevel = 180;
//     colorLevel = 255;
//     color = CRGB(255, 128, 0);
//     break;
//   case 5000:
//     whiteLevel = 85;
//     colorLevel = 255;
//     color = CRGB(255, 128, 0);
//     break;
//   case 4000:
//     whiteLevel = 29;
//     colorLevel = 255;
//     color = CRGB(255, 128, 0);
//     break;
//   case 3300:
//     whiteLevel = 15;
//     colorLevel = 255;
//     color = CRGB(255, 96, 0);
//     break;
//   case 2800:
//     whiteLevel = 8;
//     colorLevel = 255;
//     color = CRGB(255, 64, 0);
//     break;