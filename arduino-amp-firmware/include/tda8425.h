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

#ifndef _ARDUINO_AMP_TDA8425_HEADER_
#define _ARDUINO_AMP_TDA8425_HEADER_

#include "common.h"

#define SUBCMD_TDA8425_VOLUME_LEFT  0x00
#define SUBCMD_TDA8425_VOLUME_RIGHT 0x01

#define SUBCMD_TDA8425_BASS     0x02
#define SUBCMD_TDA8425_TREBLE   0x03
#define SUBCMD_TDA8425_SWITCH   0x08

#define VOLUME_TDA8425_MIN  0x00
#define VOLUME_TDA8425_MAX  0x3F

#define BASS_TDA8425_MIN    0x00
#define BASS_TDA8425_MAX    0x0F

#define TREBLE_TDA8425_MIN  0x00
#define TREBLE_TDA8425_MAX  0x0F

#define SWITCH_MUTE_TDA8425             0x20

// Stereo modes.
#define SWITCH_SPATIAL_STEREO_TDA8425   0x18
#define SWITCH_LINEAR_STEREO_TDA8425    0x08
#define SWITCH_PSEUDO_STEREO_TDA8425    0x10
#define SWITCH_MONO_TDA8425             0x00

// Channel modes.
#define SWITCH_STEREO_ONE_CHANNEL   0x06
#define SWITCH_STEREO_TWO_CHANNEL   0x07
#define SWITCH_LINE1_ONE_CHANNEL    0x02
#define SWITCH_LINE2_ONE_CHANNEL    0x04
#define SWITCH_LINE1_TWO_CHANNEL    0x03
#define SWITCH_LINE2_TWO_CHANNEL    0x05

void sendAudioProcCommand(unsigned char comAddr, unsigned char value);

void initSoundProcessor(AudioSettings *audioSettings);

void setVolume(AudioSettings *audioSettings);
void setBass(AudioSettings *audioSettings);
void setTreble(AudioSettings *audioSettings);

void muteAudio(AudioSettings *audioSettings, unsigned char isMute);
void setSwitchConfiguration(AudioSettings *audioSettings);

#endif /* _ARDUINO_AMP_TDA8425_HEADER_ */