# Lightbox-Mk1

## Overview

**Lightbox Mk1** is the code for the first proptotype of an Arduino Nano based LED art project by [Andreas Petrou](https://www.instagram.com/andreaspetrouart/). It generates fixed/alternating colours on two WS2812 programmable LEDs, and control is provided by 3 push buttons.

* Current Version: 0.1
* [Demo](https://www.instagram.com/andreaspetrouart/reel/DNEQazvoYq-/)

## Features

The mode, brightness, pallette, fixed colour/delay interval are stored in EEPROM. Reset to defaults can be done by holding button 1 on power-up.

The button operations are:
* Button 1
  * Click: Cycle through modes: ConstantColour / RandomPair / RandomPairFade / RandomSingle / RandomSingleFade.
  * Double-click: Cycle through 3 brightness levels.
  * Long-press: Cycle through colour palettes.
* Button 2
  * Click:
    * In ConstantColour mode, select previous colour in pallette.
    * Otherwise, decrease colour interval by 0.1 s.
  * Double-click: In modes other than ConstantColour, decrease colour interval by 1 s.
  * Long-press: In modes other than ConstantColour, select minium colour interval.
* Button 3
  * Click:
    * In ConstantColour mode, select next colour in pallette.
    * Otherwise, increase colour interval by 0.1 s.
  * Double-click: In modes other than ConstantColour, increase colour interval by 1 s.
  * Long-press: In modes other than ConstantColour, select maxium colour interval.

Colour Pallettes
* Primary
  * White
  * Red
  * Green
  * Blue
* Secondary 
  * White
  * Red
  * Green
  * Blue
  * Cyan
  * Magenta
  * Yellow
* Tertiary
  * White
  * Red
  * Green
  * Blue
  * Cyan
  * Magenta
  * Yellow
  * Ping
  * LightGreen
  * LightBlue
  * Maroon
  * YellowGreen
  * Navy
  * Orange
  * Purple
  * Indigo

Other notes:
* When fading between colours, the time taken increases as a proportion of the colour interval time.
* Next colour selection is random, except that the current colour is never repeated.
* Minimum colour interval is 0.1 s, maximum is 20 s.
* Some diagnostic information is printed on the serial line, eg settings and colour chnages. It is configured for 115200 bps.

## Version History

* 0.1 : Initial release.

## Code

The code was written in C++ with the Arduino framework, developed and compiled with the PlatformIO plugin for VSCode.

* main.cpp : Main code.
* colours.h / colours.cpp : Define pallette and colour functions. 
* pins.h : Define Arduino pin numberings for I/O.
* storage.h / storage.cpp : Functions to get/set settings to the EEPROM.

Libraries used:
* [FastLED](https://github.com/FastLED/FastLED)
* [OneButton](https://github.com/mathertel/OneButton)
