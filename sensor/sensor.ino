#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "system.h"
#include "mqtt.h"
#include "log.h"
#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#include "SparkFunISL29125.h"

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

static const int INTENSITY_SENSOR_ADDR = 0x48;
static const int COLOR_SENSOR_ADDR = 0x44;

SparkFun_Ambient_Light light(INTENSITY_SENSOR_ADDR);
SFE_ISL29125 RGB_sensor;

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
  Wire.begin();
  Serial.begin(115200);
  Log::get().println("Window: Setup");
  System::get().setup();
  Mqtt::get().setCallback(dispatchMessage);

  // // INTENSITY SENSOR

  // if (light.begin())
  // {
  //   Serial.println("Ready to sense some light!");
  // }
  // else
  // {
  //   Serial.println("Could not communicate with the sensor!");
  // }

  // // Again the gain and integration times determine the resolution of the lux
  // // value, and give different ranges of possible light readings. Check out
  // // hoookup guide for more info.
  // light.setGain(gain);
  // light.setIntegTime(integTime);

  // Serial.println("Reading settings...");
  // Serial.print("Gain: ");
  // float gainVal = light.readGain();
  // Serial.print(gainVal, 3);
  // Serial.print(" Integration Time: ");
  // int timeVal = light.readIntegTime();
  // Serial.println(timeVal);

  // // COLOR SENSOR
  // if (RGB_sensor.init())
  // {
  //   Serial.println("Sensor Initialization Successful\n\r");
  // }
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

  // // INTENSITY SENSOR
  // luxVal = light.readLight();

  // // COLOR SENSOR
  // // Read sensor values (16 bit integers)
  // unsigned int red = RGB_sensor.readRed();
  // unsigned int green = RGB_sensor.readGreen();
  // unsigned int blue = RGB_sensor.readBlue();

  // unsigned int maxVal = 65535;

  // unsigned long time = millis();

  // Serial.print("[");
  // Serial.print(time / 1000);
  // Serial.print("] ");
  // Serial.print("Light: ");
  // Serial.print(luxVal);
  // Serial.print(" lux, (");
  // Serial.print(red, HEX);
  // Serial.print(",");
  // Serial.print(green, HEX);
  // Serial.print(",");
  // Serial.print(blue, HEX);
  // Serial.print(") - (");
  // Serial.print((int)(1000 * (red / (float)maxVal)) / (float)1000);
  // Serial.print(",");
  // Serial.print((int)(1000 * (green / (float)maxVal)) / (float)1000);
  // Serial.print(",");
  // Serial.print((int)(1000 * (blue / (float)maxVal)) / (float)1000);
  // Serial.println(")");

  // delay(5000);
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

  // TODO start/stop measuring

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
