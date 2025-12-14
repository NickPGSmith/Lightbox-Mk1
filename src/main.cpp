//
// Name: main.cpp
// Purpose: Lightbox Mk1. Control 2 LEDs for VHS art device.
// 
// This program is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2.1 of the License, or any later version.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// Do not remove information from this header.
//
// Version History:
// 0.1    2025-12-01    Initial version.
//

#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include <OneButton.h>
#include "colours.h"
#include "pins.h"
#include "storage.h"

constexpr char firmwareVersion[] = "Lightbox Mk1 Firmware V0.1";
constexpr char firmwareLocation[] = "https://github.com/NickPGSmith/Lightbox-Mk1";
constexpr uint8_t numLEDs                   = 2;
constexpr uint16_t statusOnInterval         = 10;
constexpr uint16_t statusOffInterval        = 990;
constexpr uint16_t minColourInterval        = 100;
constexpr uint16_t maxColourInterval        = 20000;
constexpr uint16_t colourIntervalStepSmall  = 100;
constexpr uint16_t colourIntervalStepLarge  = 1000;
constexpr uint16_t fadeIntervalBoundary1    = 5000;
constexpr uint16_t fadeIntervalBoundary2    = 10000;
constexpr uint8_t fullBrightness            = 0xFF;
constexpr uint8_t medBrightness             = 0x7F;
constexpr uint8_t lowBrightness             = 0x1F;

enum class Mode : uint8_t { Constant,
                            RandomPair, RandomPairFade,
                            RandomSingle, RandomSingleFade,
                            _END_ /* Sentinel */};

//
//  ----------------------------------------------------------------------------
//  File Scope Objects
//  ----------------------------------------------------------------------------
//
namespace {
  Colours colours;
  CRGB leds[numLEDs];
  Mode mode;
  OneButton but1, but2, but3;
  
  uint32_t previousColourTimer;   // Previous millis() timer for colour cycle
  uint32_t previousStatusTimer;   // Previous millis() timer for onboard LED
  uint16_t colourInterval;        // Time to show the current colour

  bool status = false;            // State of onboard LED
  bool colourFade = false;        // True if in fade phase between colours
  bool singleState = false;       // In Single modes, true for second LED on
};

// 
//  ----------------------------------------------------------------------------
//  Functions
//  ----------------------------------------------------------------------------
//
namespace {
  // Ensure change is shown quickly by making the previous timer expire soon.
  void jumpTimer() {
    previousColourTimer = millis() - 10;
  }

  // Write interval to serial.
  void printBrightness(uint8_t brightness) {
    Serial.print(F("Brightness: "));
    Serial.println(brightness, HEX);
  }

  // Write interval to serial.
  void printInterval() {
    Serial.print(F("Interval: "));
    Serial.print(colourInterval);
    Serial.println(F(" ms"));
  }

  // Write mode to serial.
  void printMode(uint8_t mode) {
    Serial.print(F("Mode: "));
    Serial.println(static_cast<uint8_t>(mode));
  }

  // Write pallette to serial.
  void printPallette(uint8_t pallette) {
    Serial.print(F("Pallette: "));
    Serial.println(static_cast<uint8_t>(pallette));
  }

  // Cycle through modes.
  void but1Click() {
    uint8_t modeNum = (static_cast<uint8_t>(mode) + 1) % static_cast<uint8_t>(Mode::_END_);
    mode =  static_cast<Mode>(modeNum);
    Storage::setMode(modeNum);
    printMode(modeNum);
    jumpTimer();
  }

  // Toggle brightness low/medium/full.
  void but1DoubleClick() {
    uint8_t brightness = Storage::getBrightness();
    switch (brightness) {
      case (fullBrightness):
        brightness = lowBrightness;
        break;
      case (lowBrightness):
        brightness = medBrightness;
        break;
      default:
        brightness = fullBrightness;
    }
    FastLED.setBrightness(brightness);
    Storage::setBrightness(brightness);
    printBrightness(brightness);
  }
  
  // Cycle through pallettes.
  void but1LongPress() {
    uint8_t pallette = colours.nextPallette();
    Storage::setPallette(pallette);
    printPallette(pallette);
  }
  
  // Decrement colour or interval.
  void but2Click() {
    if (mode == Mode::Constant) {
      uint8_t colour = colours.decrementColour();
      Storage::setColour(colour);
      Serial.println(colour, HEX);
    } else {
      if (colourInterval - colourIntervalStepSmall >= minColourInterval) {
        colourInterval -= colourIntervalStepSmall;
      } else {
        colourInterval = minColourInterval;
      }
      printInterval();
      Storage::setInterval(colourInterval);
    }
    jumpTimer();
  }
  
  // Decrement interval by large step.
  void but2DoubleClick() {
    if (mode == Mode::Constant) {
      return;
    }

    if (colourInterval - colourIntervalStepLarge >= minColourInterval) {
      colourInterval -= colourIntervalStepLarge;
    } else {
      colourInterval = minColourInterval;
    }
    Storage::setInterval(colourInterval);
    printInterval();
    jumpTimer();
  }

  // Set minimum interval.
  void but2LongPress() {
    if (mode == Mode::Constant) {
      return;
    }

    colourInterval = minColourInterval;
    Storage::setInterval(colourInterval);
    printInterval();
    jumpTimer();
  }

  // Increment colour or interval.
  void but3Click() {
    if (mode == Mode::Constant) {
      uint8_t colour = colours.incrementColour();
      Serial.println(colour, HEX);
      Storage::setColour(colour);
    } else {
      if (colourInterval + colourIntervalStepSmall <= maxColourInterval) {
        colourInterval += colourIntervalStepSmall;
      } else {
        colourInterval = maxColourInterval;
      }
      Storage::setInterval(colourInterval);
      printInterval();
    }
    jumpTimer();
  }

  // Increment interval by large step.
  void but3DoubleClick() {
    if (mode == Mode::Constant) {
      return;
    }

    if (colourInterval + colourIntervalStepLarge <= maxColourInterval) {
      colourInterval += colourIntervalStepLarge;
    } else {
      colourInterval = maxColourInterval;
    }
    Storage::setInterval(colourInterval);
    printInterval();
    jumpTimer();
  }

  // Set maximum interval.
  void but3LongPress() {
    if (mode == Mode::Constant) {
      return;
    }

    colourInterval = maxColourInterval;
    Storage::setInterval(colourInterval);
    printInterval();
    jumpTimer();
  }
}

//
//  ----------------------------------------------------------------------------
//  Setup
//  ----------------------------------------------------------------------------
//
void setup() { 
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to connect, for native USB
  Serial.println(firmwareVersion);
  Serial.println(firmwareLocation);
  Serial.println(F("Initialising... "));
  Storage::Init();

  // Setup push buttons, active low with internal pullup.
  but1.setup(Pins::Button1, INPUT_PULLUP, true);
  but2.setup(Pins::Button2, INPUT_PULLUP, true);
  but3.setup(Pins::Button3, INPUT_PULLUP, true);
  but1.attachClick(but1Click);
  but2.attachClick(but2Click);
  but3.attachClick(but3Click);
  but1.attachDoubleClick(but1DoubleClick);
  but2.attachDoubleClick(but2DoubleClick);
  but3.attachDoubleClick(but3DoubleClick);
  but1.attachLongPressStop(but1LongPress);
  but2.attachLongPressStop(but2LongPress);
  but3.attachLongPressStop(but3LongPress);

  // Reset to defaults if button 1 held during startup.
  if (!digitalRead(Pins::Button1)) {
    Storage::resetToDefaults();
    Serial.println(F("Loading defaults... "));
  }

  // Setup LEDs and settings from EEPROM.
  pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2812, Pins::LED_Data, GRB>(leds, numLEDs);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450); // For 500 mA USB PSU
  uint8_t tmp = Storage::getBrightness();
  FastLED.setBrightness(tmp);
  printBrightness(tmp);
  tmp = Storage::getPallette();
  colours.setPallette(tmp);
  printPallette(tmp); 
  tmp = Storage::getColour();
  colours.setColour(tmp);
  Serial.print(F("Colour: "));
  Serial.println(tmp, HEX);
  colourInterval = Storage::getInterval();
  printInterval();
  tmp = Storage::getMode();
  mode = static_cast<Mode>(tmp);
  printMode(tmp);
  
  Serial.println(F("Done."));
  delay(2000);
  jumpTimer();
}

// 
//  ----------------------------------------------------------------------------
//  Main Loop
//  ----------------------------------------------------------------------------
//
void loop() {
  // Update delay timer.
  uint32_t currentTimer = millis();
  uint32_t deltaTimer = currentTimer - previousColourTimer;

  // Update onboard LED status.
  if (currentTimer - previousStatusTimer >= (status ? statusOnInterval : statusOffInterval)) {
    previousStatusTimer = currentTimer;
    status = !status;
    digitalWrite(LED_BUILTIN, status ? HIGH : LOW);
  }

  // Button processing.
  but1.tick();
  but2.tick();
  but3.tick();

  // Constant colour on both LEDs regardless of the timer.
  if (mode == Mode::Constant) {
    leds[0] = colours.getColour();
    leds[1] = colours.getColour();
    FastLED.show();
    return;
  }

  // Show current colour.
  if (deltaTimer < colourInterval) {
    if (mode == Mode::RandomSingle || mode == Mode::RandomSingleFade) {
      if (singleState) {
        leds[0] = CRGB::Black;
        leds[1] = colours.getColour();
      } else {
        leds[0] = colours.getColour();
        leds[1] = CRGB::Black;
      }
    } else {
      leds[0] = colours.getColour();
      leds[1] = colours.getColour();
    }
    FastLED.show();
    return;
  }

  // For non-fade modes, time to change colour.
  if ((mode == Mode::RandomPair || mode == Mode::RandomSingle)) {
    colours.randomColour();
    Serial.println(colours.getColourNum(), HEX);
    singleState = !singleState;
    previousColourTimer = currentTimer;
    // LEDs will be updated in next loop iteration.
    return;
  }

  // For longer intervals, use a larger fade interval.
  uint16_t fadeInterval = 0;
  if (colourInterval <= fadeIntervalBoundary1) {
    fadeInterval = colourInterval / 4;
  } else if (colourInterval <= fadeIntervalBoundary2) {
    fadeInterval = colourInterval / 2;
  } else {
    fadeInterval = colourInterval;
  }

  // Time to start colour fade.
  if (deltaTimer < (colourInterval + fadeInterval)) {
    if (!colourFade) {
      colourFade = true;
      colours.randomColour();
      Serial.println(colours.getColourNum(), HEX);
    }
    uint8_t fadeFraction = (deltaTimer - colourInterval) * 0xFF / fadeInterval;
    CRGB fadeColour = colours.getPreviousColour().lerp8(colours.getColour(), fadeFraction);
    if (mode == Mode::RandomSingle || mode == Mode::RandomSingleFade) {
      if (singleState) {
        leds[0] = CRGB::Black;
        leds[1] = fadeColour;
      } else {
        leds[0] = fadeColour;
        leds[1] = CRGB::Black;
      }
    } else {
      leds[0] = fadeColour;
      leds[1] = fadeColour;
    }
    FastLED.show();
    return;
  }

  // After colour fade: update timer.
  colourFade = false;
  singleState = !singleState;
  previousColourTimer = currentTimer;
}