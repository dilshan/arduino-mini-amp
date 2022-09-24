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
