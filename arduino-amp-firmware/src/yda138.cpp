#include "yda138.h"
#include "common.h"

#include <Arduino.h>

void setPowerAmpMute(unsigned char isMute)
{
    digitalWrite(YDA138_MUTE_CNT, (isMute) ? HIGH : LOW);
}

void setAudioOutputMode(unsigned char mode)
{
    digitalWrite(YDA138_HEADPHONE_MODE, (mode == AUDIO_OUT_SPEAKER) ? HIGH : LOW);
}