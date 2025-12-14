#pragma once

//
// Name: pins.h
// Purpose: Define pins (Arduino numbering).
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

namespace Pins {
  constexpr uint8_t LED_Data =    2;      // D2: Output for LED data
  constexpr uint8_t Button1 =     5;      // D5: Input for button 1
  constexpr uint8_t Button2 =     4;      // D4: Input for button 2
  constexpr uint8_t Button3 =     3;      // D3: Input for button 3
}