/*************************************************************************
  This file is part of the Arduino Mini Amplifier project.

  Copyright (c) 2025 Dilshan R Jayakody [jayakody2000lk at gmail d0t com]

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:
  
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************/

#ifndef _ARDUINO_AMP_DISPLAY_UTIL_HEADER_
#define _ARDUINO_AMP_DISPLAY_UTIL_HEADER_

#include "common.h"

void clearRow(unsigned char row);
void displayVolumeLevel(unsigned char lvlVolume);
void displayMenuItem(SettingsMenuState *menuState, AudioSettings *audioSettings, unsigned char *outputMode);
void showMute();

#endif/* _ARDUINO_AMP_DISPLAY_UTIL_HEADER_ */