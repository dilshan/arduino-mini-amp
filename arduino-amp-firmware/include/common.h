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

#ifndef _ARDUINO_AMP_COMMON_HEADER_
#define _ARDUINO_AMP_COMMON_HEADER_

// Pin mapping for HD44780 - 16x2 LCD
#define LCD_RS  2
#define LCD_EN  3
#define LCD_D4  4   
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7

// Pin mapping for input buttons.
#define SWITCH_ACTION   8
#define SWITCH_UP       9
#define SWITCH_DOWN     10
#define SWITCH_MUTE     11

#define YDA138_MUTE_CNT         12
#define YDA138_HEADPHONE_MODE   13

#define TRUE  0xFF
#define FALSE 0x00

#define AUDIO_OUT_SPEAKER   0x00
#define AUDIO_OUT_HEADPHONE 0x01

#define IDLE_TIMEOUT        300
#define IDLE_MENU_TIMEOUT   400

#define EEPROM_ADDR_VOLUME  0x00
#define EEPROM_ADDR_BASS    0x01
#define EEPROM_ADDR_TREBLE  0x02
#define EEPROM_ADDR_SWCONF  0x03
#define EEPROM_ADDR_OUTPUT  0x04

typedef enum
{
    INPUT_CHANNEL,
    LVL_BASS,
    LVL_TREBLE,
    MODE_STEREO,
    OUTPUT_MODE,
    EXIT

} SettingsMenuState;

typedef struct 
{
    unsigned char volume;
    unsigned char bass;
    unsigned char treble;
    unsigned char switchConfig;    
} AudioSettings;


#endif /* _ARDUINO_AMP_COMMON_HEADER_ */
