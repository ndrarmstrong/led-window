#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "system.h"
#include "mqtt.h"
#include "leds.h"
#include "log.h"
#include "modeSystem.h"
#include "modeSelfTest.h"
#include "modeRaw.h"

/**
 * @brief LED window display modes.
 */
enum Modes
{
  /**
   * @brief Window off.
   */
  OFF = 0,

  /**
   * @brief Startup mode - equivalent to system followed by primary mode
   */
  STARTUP = 1,

  /**
   * @brief System mode - displays system status
   */
  SYSTEM = 2,

  /**
   * @brief Self test mode - shows LED strip channels
   */
  SELF_TEST = 3,

  /**
   * @brief Raw mode - renders color exactly as given
   */
  RAW = 4,

  /**
   * @brief Daylight mode - renders outside color
   */
  DAYLIGHT = 5,

  /**
   * @brief Stained glass mode - emulate a stained glass window
   */
  STAINED_GLASS = 6
};

/**
 * @brief Primary operating mode; activates after Wi-Fi connects.
 */
const Modes primaryMode = Modes::OFF;

/**
 * @brief Current operating mode.
 */
Modes currentMode = Modes::STARTUP;

/**
 * @brief System mode instance.
 */
ModeSystem modeSystem(&System::get(), &Mqtt::get(), &Leds::get());

/**
 * @brief Self test mode instance.
 */
ModeSelfTest modeSelfTest(&Leds::get());

/**
 * @brief Raw mode instance.
 */
ModeRaw modeRaw(&Leds::get());

/**
 * @brief Setup function - runs once on start
 */
void setup()
{
  Serial.begin(115200);
  Log::get().println("Window: Setup");
  System::get().setup();
  Leds::get().setup();
  modeSystem.start();
  Mqtt::get().setCallback(dispatchMessage);
}

/**
 * @brief Main microprocessor loop.\
 */
void loop()
{
  System::get().loop();
  Mqtt::get().loop(System::get().isAssociated());

  if (currentMode != Modes::SYSTEM && System::get().isAssociated() && System::get().getOtaState() != OtaState::Disabled)
  {
    // If OTA is enabled, force into system mode
    switchModes(Modes::SYSTEM);
  }
  else if (currentMode == Modes::STARTUP && System::get().isAssociated() && Mqtt::get().isConnected())
  {
    // If we've completed startup, switch into the primary mode
    switchModes(primaryMode);
  }
  else if (!System::get().isAssociated() && currentMode != Modes::STARTUP)
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

  Log::get().print("Window: Switching from mode ");
  Log::get().print(currentMode);
  Log::get().print(" to mode ");
  Log::get().println(nextMode);

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
  default:
    Leds::get().reset();
  }

  currentMode = nextMode;
}

/**
 * @brief Dispatch incoming MQTT messages
 * 
 * @param topic Message topic
 * @param payload Message payload
 * @param length Message payload length
 */
void dispatchMessage(char *topic, byte *payload, unsigned int length)
{
  // Sending anything (including log messages now!) will overwrite;
  // make a copy before proceeding
  String topicStr = String(topic);
  byte *payloadCopy = (byte *)malloc(length);
  memcpy(payloadCopy, payload, length);

  Log::get().print("Window: Recieved message on topic ");
  Log::get().println(topicStr);

  if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_MODE) == topicStr)
  {
    onModeMessage(payloadCopy, length);
  }
  else if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_SYS) == topicStr)
  {
    System::get().onSysMessage(payloadCopy, length);
  }
  else if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_DESCRIBE) == topicStr)
  {
    System::get().onDescribeMessage(payloadCopy, length);
  }
  else
  {
    Log::get().print("Window: Unknown topic");
    Log::get().println(topic);
  }

  free(payloadCopy);
}

/**
 * @brief Change window mode based on mode message, and respond
 * @param payload Message payload
 * @param length Message payload length
 */
void onModeMessage(byte *payload, unsigned int length)
{
  StaticJsonDocument<Config::MQTT_MESSAGE_SIZE_B> reqDoc;
  bool success = false;

  DeserializationError err = deserializeJson(reqDoc, payload, length);

  if (err == DeserializationError::Ok)
  {
    switchModes((Modes)reqDoc["mode"]);
    success = true;
  }
  else
  {
    Log::get().print("Window: Unable to parse mode message: ");
    Log::get().println(err.c_str());
  }

  Mqtt::get().acknowledge(Config::MQTT_MSG_TOPIC_MODE, success);
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