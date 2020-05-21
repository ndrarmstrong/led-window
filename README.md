# LED Window

Source code for the window, light sensor, service, and UI of my LED Window hobby project.

## Motivation

When I watched the
[DIY Perks video on artificial daylight using old monitors](https://www.youtube.com/watch?v=8JrqH2oOTK4), I immediately
started seeing every dark spot of my house as problem that could be fixed.  But I really wanted to go one step further
-- rather than just a light that could turn on and off, would it be possible to control the color and intensity to
match the current outside conditions?

While I was at university, I was had to take a fair number of hardware/circuit/embedded/FPGA courses... and I never
really got it. Software was my thing, and I've done exclusively that ever since. But the increasing availability of
hobbiest electronics resources, cheap IoT hardware, and working on the software side of combined hardware-software
projects professionally led me to believe I might just get it if I gave it another shot.

With those two factors, I had the perfect project -- conceptually simple, but with enough non-trivial parts that I'd
be forced to understand rather than copy.  And there were great resources for most of the individual steps -- but no
end-to-end guide to follow.

I just might learn enough electronics knowledge to be dangerous.

## Disclaimers and Warnings

This code was written for my house, without reuse as a goal.  Much of the code -- particularly for the microcontrollers
-- is specific to my hardware configuration and would require rework to support different LED strip combinations,
wiring, and sensors.

Also, while I write software professionally, my experience is with higher-stack languages. I'm confident the TypeScript
is decent, but my C++ was never great.  While I'm happy with my result (especially compared with a lot of the Arduino
code out there), I'm certain I did some bad things along the way.  Don't think of my microcontroller code as idomatic
C++ for embedded!

Fork, derive, experiment with this code -- but be aware that it will likely need to be modified for your purposes.

I will be maintaining this code only as much as necessary to keep my window running in my home.  I document primarily
to save me head-scratching a couple years down the road when I need to make a change and forget what I did. 😀

## Development environment

The window and sensor are built using the Arduino toolset (1.8.12), with either the official Arduino IDE or with the
Ardiuno extension for Visual Studio Code (in which most of the code was written).

The service and UI are built with a typical node.js toolchain.

### Arduino development

You will need the ESP8266 (2.7.0) board package with its associated libraries.

You will need to import the following libraries in addition to the board:

* Adafruit Unified Sensor (1.1.2) and DHT sensor library (1.3.9)
* FastLED (3.3.3)

### node.js development

The exact configuration needed for building the service and UI are contained within the included development container.

To build the development container:

```console
$ docker-compose build
```

To run the development container:

```console
$ docker-compose run --rm window-dev
```

## Hardware

### Window

The window consists of:

* ESP8266 nodeMCU - 1.0 (ESP-12E)
* Marswell Pure White 12V LED strip
* BTF-Lighting WS2811 RGB Addressable 12V LED strip
* TO-220 N-Channel power MOSFETs with heatsinks (RFP30N06LE)
* Adjustable step-down DC-DC voltage regulator (MP1584EN)
* 3.3V to 4V logic-level shifter
* 10KΩ resistors for pull-down
* 10Ω resistor for current-limiting MOSFET gate
* LED with 220Ω current-limiting resistor
* 2 pin and 3 pin screw terminals
* Prototype board and wire