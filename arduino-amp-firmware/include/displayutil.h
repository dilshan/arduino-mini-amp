#ifndef _ARDUINO_AMP_DISPLAY_UTIL_HEADER_
#define _ARDUINO_AMP_DISPLAY_UTIL_HEADER_

#include "common.h"

void clearRow(unsigned char row);
void displayVolumeLevel(unsigned char lvlVolume);
void displayMenuItem(SettingsMenuState *menuState, AudioSettings *audioSettings, unsigned char *outputMode);
void showMute();

#endif/* _ARDUINO_AMP_DISPLAY_UTIL_HEADER_ */