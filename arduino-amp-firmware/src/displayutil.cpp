#include "common.h"
#include "displayutil.h"
#include "tda8425.h"

#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

void showMute()
{
    lcd.clear();
    lcd.print("     MUTE     ");
}

void clearRow(unsigned char row)
{
    char tempPos;

    lcd.setCursor(0, row);

    for(tempPos = 0; tempPos < 16; tempPos++)
    {
        lcd.write(' ');
    }

    // Return cursor to the home position of the specified row.
    lcd.setCursor(0, row);
}

void displayVolumeLevel(unsigned char lvlVolume)
{
    lcd.clear();
    lcd.print("Volume: ");
    lcd.print(lvlVolume);
}

void displayMenuItem(SettingsMenuState *menuState, AudioSettings *audioSettings, unsigned char *outputMode)
{
    unsigned char tempBuffer;
    
    clearRow(1);

    switch(*menuState)
    {
        case INPUT_CHANNEL:     // Input mode selection.
            lcd.print("Input: ");   
            tempBuffer = (audioSettings->switchConfig) & 0x07;
            switch (tempBuffer)
            {
                case SWITCH_STEREO_ONE_CHANNEL: // Line L+R.
                    lcd.print("BT L+R");
                    break;
                case SWITCH_STEREO_TWO_CHANNEL: // Bluetooth L+R.
                    lcd.print("Line L+R");
                    break;
                case SWITCH_LINE1_ONE_CHANNEL:  // Line Left.
                    lcd.print("BT L");
                    break;
                case SWITCH_LINE2_ONE_CHANNEL:  // Bluetooth Left.
                    lcd.print("BT R");
                    break;
                case SWITCH_LINE1_TWO_CHANNEL:  // Line Right.
                    lcd.print("Line L");
                    break;
                case SWITCH_LINE2_TWO_CHANNEL:  // Bluetooth Right.
                    lcd.print("Line R");
                    break;
            }
            break;
        case LVL_BASS:      // Bass level.
            lcd.print("Bass: "); 
            lcd.print(((char)(audioSettings->bass)) - 6);
            break;
        case LVL_TREBLE:    // Treble level.
            lcd.print("Treble: "); 
            lcd.print(((char)(audioSettings->treble)) - 6);
            break;
        case MODE_STEREO:   // Stereo configuration.
            lcd.print("Channel: ");
            tempBuffer = (audioSettings->switchConfig) & 0x18;
            switch (tempBuffer)
            {
                case SWITCH_SPATIAL_STEREO_TDA8425:
                    lcd.print("Spatial");
                    break;
                case SWITCH_LINEAR_STEREO_TDA8425:
                    lcd.print("Stereo");
                    break;
                case SWITCH_PSEUDO_STEREO_TDA8425:
                    lcd.print("Pseudo");
                    break;
                case SWITCH_MONO_TDA8425:
                    lcd.print("Mono");
                    break;
            }
            break;
        case OUTPUT_MODE:   // Audio output mode.
            lcd.print("Output: ");
            lcd.print(((*outputMode) == AUDIO_OUT_SPEAKER) ? "Speaker" : "HPhone");
            break;
        case EXIT:          // Exit from settings menu.
            lcd.print("Exit");
            break;
    }
}