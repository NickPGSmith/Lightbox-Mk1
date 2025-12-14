//
// Name: storage::cpp
// Purpose: Wrapper functions for reading/wring data to EEPROM.
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
#include <EEPROM.h>
#include "storage.h"

namespace Storage {
  constexpr uint8_t defaultBrightness = 0xFF;
  constexpr uint8_t defaultColour = 0;
  constexpr uint16_t defaultInterval = 1000;
  constexpr uint8_t defaultMode = 0;
  constexpr uint8_t defaultPallette = 0;

  // EEPROM initialisation marker: can be anything other than 0xFF.
  constexpr uint8_t initialisedVal = 0;

  // Addresses of storage locations in EEPROM.
  constexpr uint16_t addrInitialised = 0;
  constexpr uint16_t addrBrightness = addrInitialised + sizeof(uint8_t);
  constexpr uint16_t addrColour = addrBrightness + sizeof(uint8_t);
  constexpr uint16_t addrInterval = addrColour + sizeof(uint8_t);
  constexpr uint16_t addrMode = addrInterval + sizeof(uint16_t);
  constexpr uint16_t addrPallette = addrMode + sizeof(uint8_t);

  //
  // Check whether EEPROM has been initialised (new EEPROM contains all 0xFF).
  //
  void Init() {
    if (EEPROM.read(addrInitialised) != initialisedVal)
      resetToDefaults();
  }

  //
  // Reset all settings to their default values.
  //
  void resetToDefaults() {
    EEPROM.put(addrInitialised, initialisedVal);
    setBrightness(defaultBrightness);
    setColour(defaultColour);
    setInterval(defaultInterval);
    setMode(defaultMode);
    setPallette(defaultPallette);
  }

  //
  // Get/Set the brightness.
  //
  uint8_t const getBrightness() {
    uint8_t value;
    
    EEPROM.get(addrBrightness, value);
    return value;
  }

  void setBrightness(uint8_t value) {
    EEPROM.put(addrBrightness, value);
  }

  //
  // Get/Set the colour (constant mode).
  //
  uint8_t const getColour() {
    uint8_t value;
    
    EEPROM.get(addrColour, value);
    return value;
  }

  void setColour(uint8_t value) {
    EEPROM.put(addrColour, value);
  }

  //
  // Get/Set the delay interval.
  //
  uint16_t const getInterval() {
    uint16_t value;
    
    EEPROM.get(addrInterval, value);
    return value;
  }

  void setInterval(uint16_t value) {
    EEPROM.put(addrInterval, value);
  }

  //
  // Get/Set the mode.
  //
  const uint8_t getMode() {
    uint8_t value;
    
    EEPROM.get(addrMode, value);
    return value;
  }

  void setMode(uint8_t value) {
    EEPROM.put(addrMode, value);
  }

  //
  // Get/Set the pallette number.
  //
  const uint8_t getPallette() {
    uint8_t value;
    
    EEPROM.get(addrPallette, value);
    return value;
  }

  void setPallette(uint8_t value) {
    EEPROM.put(addrPallette, value);
  }
}