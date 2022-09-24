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