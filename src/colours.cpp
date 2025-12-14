//
// Name: colours.cpp
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
// Version History:
// 0.1    2025-12-01    Initial version.
//

#include <stdint.h>
#include <Arduino.h>
#include "colours.h"

namespace {
  constexpr uint8_t numPallettes = 3;
}

//
// Set the palette.
//
void Colours::setPallette(uint8_t palletteNum) {
  palletteNum_ = (palletteNum < numPallettes) ? palletteNum : 0;
  
  // Set colour to first if moving to a palette with fewer colours than current.
  if (colourNum_ >= palletteSizes_[palletteNum_]) {
    colourNum_ = 0;
  }
}

//
// Move to next palette.
//
uint8_t Colours::nextPallette() {
  uint8_t newPallette = (palletteNum_ + 1) % numPallettes;
  setPallette(newPallette);

  return newPallette;
}

//
// Set colour from the current palette.
//
void Colours::setColour(uint8_t colourNum) {
  colourNum_ = (colourNum < palletteSizes_[palletteNum_]) ? colourNum : 0;
}

//
// Increment the colour number, with wrap around.
//
uint8_t Colours::incrementColour() {
  uint8_t tmp = ++colourNum_; // Use tmp to avoid compiler wanrning
  colourNum_ = tmp % palletteSizes_[palletteNum_];
  
  return colourNum_;
}

//
// Decrement the colour number, with wrap around.
//
uint8_t Colours::decrementColour() {
  uint8_t tmp = --colourNum_; // Use tmp to avoid compiler wanrning
  colourNum_ = tmp % palletteSizes_[palletteNum_];

  return colourNum_;
};

//
// Choose a new random colour from the current palette, but avoid existing colour.
//
CRGB Colours::randomColour() {
  uint8_t newCol;

  do {
    newCol = random(palletteSizes_[palletteNum_]);
  } while (newCol == colourNum_);
  previousColour_ = pallettes_[palletteNum_][colourNum_];
  colourNum_ = newCol;

  return pallettes_[palletteNum_][colourNum_];
}