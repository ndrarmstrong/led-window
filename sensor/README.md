# Sensor Microcontroller

This folder contains the code for the light sensor microcontroller

## Config

A config file named `env.h` should be placed in this directory containing any sensitive values that should not be
checked into the repository.  When checking out a fresh repo, copy `env.sample.h` to `env.h` and edit as appropriate.

## Build

This repo can be built using the Arduino IDE or VS Code with the Arduino extension

## Target hardware

This project was written for the ESP8266 nodeMCU.

## Sensor libraries

SparkFun ISL29125 - https://github.com/sparkfun/SparkFun_ISL29125_Breakout_Arduino_Librar
SparkFun VEML6030 - https://github.com/sparkfun/SparkFun_Ambient_Light_Sensor_Arduino_Library