#pragma once

//
// Name: storage::h
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

#include <stdint.h>

namespace Storage
{
  void Init();
  void resetToDefaults();

  const uint8_t getBrightness();
  void setBrightness(uint8_t value);

  const uint8_t getColour();
  void setColour(uint8_t value);

  const uint16_t getInterval();
  void setInterval(uint16_t value);

  const uint8_t getMode();
  void setMode(uint8_t value);

  const uint8_t getPallette();
  void setPallette(uint8_t value);
};