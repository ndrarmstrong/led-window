#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"
#include "SparkFunISL29125.h"

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

  // INTENSITY SENSOR

  if (light.begin())
  {
    Serial.println("Ready to sense some light!");
  }
  else
  {
    Serial.println("Could not communicate with the sensor!");
  }

  // Again the gain and integration times determine the resolution of the lux
  // value, and give different ranges of possible light readings. Check out
  // hoookup guide for more info.
  light.setGain(gain);
  light.setIntegTime(integTime);

  Serial.println("Reading settings...");
  Serial.print("Gain: ");
  float gainVal = light.readGain();
  Serial.print(gainVal, 3);
  Serial.print(" Integration Time: ");
  int timeVal = light.readIntegTime();
  Serial.println(timeVal);

  // COLOR SENSOR
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
}

void loop()
{
  // INTENSITY SENSOR
  luxVal = light.readLight();

  // COLOR SENSOR
  // Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();

  unsigned int maxVal = 65535;

  unsigned long time = millis();

  Serial.print("[");
  Serial.print(time / 1000);
  Serial.print("] ");
  Serial.print("Light: ");
  Serial.print(luxVal);
  Serial.print(" lux, (");
  Serial.print(red, HEX);
  Serial.print(",");
  Serial.print(green, HEX);
  Serial.print(",");
  Serial.print(blue, HEX);
  Serial.print(") - (");
  Serial.print((int)(1000 * (red / (float)maxVal)) / (float)1000);
  Serial.print(",");
  Serial.print((int)(1000 * (green / (float)maxVal)) / (float)1000);
  Serial.print(",");
  Serial.print((int)(1000 * (blue / (float)maxVal)) / (float)1000);
  Serial.println(")");

  delay(5000);
}