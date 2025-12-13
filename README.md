# Lightbox-Mk1

## Overview

**Lightbox Mk1** is the first proptotype of an Arduino Nano based LED art project. It generates fixed/alternating colours on two WS2812 programmable LEDs, and control is provided by 3 push buttons.

TODO: Link to Instagram.

Current Version:
* 0.1

## Features

* ?

## Code

The code was written in C++ with the Arduino framework, developed and compiled with the PlatformIO plugin for VSCode.

* main.cpp : Main code.
* colours.h / colours.cpp : Define pallette and colour functions. 
* pins.h : Define Arduino pin numberings for I/O.
* storage.h / storage.cpp : Functions to get/set settings to the EEPROM.

Libraries used:
* [FastLED](https://github.com/FastLED/FastLED)
* [OneButton](https://github.com/mathertel/OneButton)
