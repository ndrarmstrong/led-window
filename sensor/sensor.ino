#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "system.h"
#include "mqtt.h"
#include "lightSensor.h"
#include "log.h"

/**
 * @brief LED window display modes.
 */
enum Modes
{
  /**
   * @brief Sensor is measuring normally.
   */
  MEASURE = 0,

  /**
   * @brief Startup mode - equivalent to system followed by primary mode
   */
  STARTUP = 1,

  /**
   * @brief System mode - not measuring
   */
  SYSTEM = 2
};

/**
 * @brief Primary operating mode; activates after Wi-Fi connects.
 */
const Modes primaryMode = Modes::MEASURE;

/**
 * @brief Current operating mode.
 */
Modes currentMode = Modes::STARTUP;

static const int COLOR_SENSOR_ADDR = 0x44;

// INTENSITY SENSOR SETTING

// Possible values: .125, .25, 1, 2
// Both .125 and .25 should be used in most cases except darker rooms.
// A gain of 2 should only be used if the sensor will be covered by a dark
// glass.
float gain = .125;

// Possible integration times in milliseconds: 800, 400, 200, 100, 50, 25
// Higher times give higher resolutions and should be used in darker light.
int integTime = 100;
long luxVal = 0;

void setup()
{
  Serial.begin(115200);
  Log::get().println("Window: Setup");
  System::get().setup();
  Mqtt::get().setCallback(dispatchMessage);
}

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

  Log::get().print("Sensor: Switching from mode ");
  Log::get().print(currentMode);
  Log::get().print(" to mode ");
  Log::get().println(nextMode);

  if (nextMode == Modes::MEASURE)
  {
    LightSensor::get().start();
  }
  else
  {
    LightSensor::get().stop();
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

  Log::get().print("Sensor: Recieved message on topic ");
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
  else if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_LIGHT_LEVEL) == topicStr)
  {
    LightSensor::get().onLightLevelMessage(payloadCopy, length);
  }
  else if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_INTERVAL) == topicStr)
  {
    LightSensor::get().onIntervalMessage(payloadCopy, length);
  }
  else if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_LUX) == topicStr)
  {
    LightSensor::get().onLuxMessage(payloadCopy, length);
  }
  else if (Mqtt::get().deviceReqTopic(Config::MQTT_MSG_TOPIC_RGB) == topicStr)
  {
    LightSensor::get().onRgbMessage(payloadCopy, length);
  }
  else
  {
    Log::get().print("Sensor: Unknown topic");
    Log::get().println(topicStr);
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
    Log::get().print("Sensor: Unable to parse mode message: ");
    Log::get().println(err.c_str());
  }

  Mqtt::get().acknowledge(Config::MQTT_MSG_TOPIC_MODE, success);
}
