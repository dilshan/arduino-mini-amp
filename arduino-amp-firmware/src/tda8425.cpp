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

#include "tda8425.h"
#include "common.h"

#include <Arduino.h>
#include <Wire.h>

#define TDA8425_ADDRESS 0x41

void sendAudioProcCommand(unsigned char subAddr, unsigned char value)
{
    Wire.beginTransmission(TDA8425_ADDRESS);
    Wire.write(subAddr);
    Wire.write(value);
    Wire.endTransmission();
}

void initSoundProcessor(AudioSettings *audioSettings)
{
    // Mute audio and enable linear stereo.
    audioSettings->switchConfig = (SWITCH_MUTE_TDA8425 | SWITCH_LINEAR_STEREO_TDA8425 | SWITCH_STEREO_TWO_CHANNEL | 0xC0);
    sendAudioProcCommand(SUBCMD_TDA8425_SWITCH, audioSettings->switchConfig);
    
    // Set volume level to minimum position (-80dB).
    audioSettings->volume = VOLUME_TDA8425_MIN;
    sendAudioProcCommand(SUBCMD_TDA8425_VOLUME_LEFT, (VOLUME_TDA8425_MIN | 0xC0));
    sendAudioProcCommand(SUBCMD_TDA8425_VOLUME_RIGHT, (VOLUME_TDA8425_MIN | 0xC0));

    // Set bass and treble levels to 0dB.
    audioSettings->bass = 0x06;
    sendAudioProcCommand(SUBCMD_TDA8425_BASS, (0x06 | 0xF0));
    
    audioSettings->treble = 0x06;
    sendAudioProcCommand(SUBCMD_TDA8425_TREBLE, (0x06 | 0xF0));    
}

void setVolume(AudioSettings *audioSettings)
{
    audioSettings->volume = (audioSettings->volume > VOLUME_TDA8425_MAX) ? VOLUME_TDA8425_MAX : audioSettings->volume;

    // Set volume on both left and right channels.
    sendAudioProcCommand(SUBCMD_TDA8425_VOLUME_LEFT, (audioSettings->volume | 0xC0));
    sendAudioProcCommand(SUBCMD_TDA8425_VOLUME_RIGHT, (audioSettings->volume | 0xC0));
}

void setBass(AudioSettings *audioSettings)
{
    audioSettings->bass = (audioSettings->bass > BASS_TDA8425_MAX) ? BASS_TDA8425_MAX : audioSettings->bass;
    sendAudioProcCommand(SUBCMD_TDA8425_BASS, (audioSettings->bass | 0xF0));
}

void setTreble(AudioSettings *audioSettings)
{
    audioSettings->treble = (audioSettings->treble > TREBLE_TDA8425_MAX) ? TREBLE_TDA8425_MAX : audioSettings->treble;
    sendAudioProcCommand(SUBCMD_TDA8425_TREBLE, (audioSettings->treble | 0xF0));
}

void muteAudio(AudioSettings *audioSettings, unsigned char isMute)
{
    if(isMute)
    {
        audioSettings->switchConfig = audioSettings->switchConfig | SWITCH_MUTE_TDA8425;
    }
    else
    {
        audioSettings->switchConfig = audioSettings->switchConfig & (~SWITCH_MUTE_TDA8425);
    }

    sendAudioProcCommand(SUBCMD_TDA8425_SWITCH, audioSettings->switchConfig);
}

void setSwitchConfiguration(AudioSettings *audioSettings)
{
    sendAudioProcCommand(SUBCMD_TDA8425_SWITCH, audioSettings->switchConfig);
}
