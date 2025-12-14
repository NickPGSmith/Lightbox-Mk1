#pragma once

//
// Name: colours.h
// Purpose: Class to hold colour palette and functions.
// 
// This program is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2.1 of the License, or any later version.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// Do not remove information from this header.
//
// NOTE: See here for colour names:
// https://github.com/FastLED/FastLED/wiki/Pixel-reference
//
// Version History:
// 0.1    2025-12-01    Initial version.
//

#include <stdint.h>
#include <FastLED.h>

class Colours {
  public:
    Colours() { setPallette(0); setColour(0); };

    const uint8_t getPallette() { return palletteNum_; }
    void setPallette(uint8_t palletteNum);
    uint8_t nextPallette();

    const uint8_t getColourNum() { return colourNum_; }
    void setColour(uint8_t colourNum);
    uint8_t incrementColour();
    uint8_t decrementColour();
    CRGB getColour() { return pallettes_[palletteNum_][colourNum_]; }
    CRGB getPreviousColour() { return previousColour_; };
    CRGB randomColour();

  private:
    static const uint8_t numPallettes_ = 3;
    const uint8_t palletteSizes_[numPallettes_] = { pallette0Size, pallette1Size, pallette2Size };
    const CRGB* const pallettes_[numPallettes_] = { pallette0_, pallette1_, pallette2_ };
    uint8_t palletteNum_;
    uint8_t colourNum_;
    CRGB previousColour_ = CRGB::Black;

    // Pallette 0: White and primary colours.
    static const uint8_t pallette0Size = 4;
    const CRGB pallette0_[pallette0Size] = {
      CRGB::White, CRGB::Red, CRGB::Green, CRGB::Blue
    };

    // Pallette 1: pallette 0 plus desaturated primary colours.
    static const uint8_t pallette1Size = 7;
    const CRGB pallette1_[pallette1Size] = {
      CRGB::White, CRGB::Red, CRGB::Green, CRGB::Blue,
      CRGB::Cyan, CRGB::Magenta, CRGB::Yellow
    };

    // Pallette 2: pallette 1 plus some others.
    static const uint8_t pallette2Size = 16;
    const CRGB pallette2_[pallette2Size] = {
      CRGB::White, CRGB::Red, CRGB::Green, CRGB::Blue,
      CRGB::Cyan, CRGB::Magenta, CRGB::Yellow,
      CRGB::Pink, CRGB::LightGreen, CRGB::LightBlue,
      CRGB::Maroon, CRGB::YellowGreen, CRGB::Navy,
      CRGB::Orange, CRGB::Chocolate, CRGB::Indigo
    };
};