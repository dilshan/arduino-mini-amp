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

#include "common.h"
#include "tda8425.h"
#include "yda138.h"
#include "displayutil.h"

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <fix_fft.h>

#define ANALYZER_SAMPLES        128
#define ANALYZER_COLUMNS        16
#define LCD_MAX_COLUMN_HEIGHT   16

unsigned char btnState_Action, btnState_Up, btnState_Down, btnState_Mute;
unsigned short idleCounter;
unsigned char audioOutMode, isAudioMute, isLCDShowMute;
AudioSettings audioSettings;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

char analogData[ANALYZER_SAMPLES];
char imgData[ANALYZER_SAMPLES];
int graphData[ANALYZER_SAMPLES];

// Spectrum analyzer (bar-graph) character configuration.
unsigned char graphLine1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};
unsigned char graphLine2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F};
unsigned char graphLine3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F};
unsigned char graphLine4[] = {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char graphLine5[] = {0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char graphLine6[] = {0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char graphLine7[] = {0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

void saveConfiguration(AudioSettings *audioSettings, unsigned char *outputMode)
{
    // Save audio configurations.
    EEPROM.update(EEPROM_ADDR_VOLUME, audioSettings->volume);
    EEPROM.update(EEPROM_ADDR_BASS, audioSettings->bass);
    EEPROM.update(EEPROM_ADDR_TREBLE, audioSettings->treble);
    EEPROM.update(EEPROM_ADDR_SWCONF, audioSettings->switchConfig);

    // Save audio output mode.
    EEPROM.update(EEPROM_ADDR_OUTPUT, *outputMode);
}

unsigned char loadLastConfiguration(AudioSettings *audioSettings, unsigned char *outputMode)
{
    AudioSettings tempSettings;
    unsigned char tempOutputMode;
    unsigned char isValueUpdate = FALSE;
    
    // Load audio configuration.
    tempSettings.volume = EEPROM.read(EEPROM_ADDR_VOLUME);
    tempSettings.bass = EEPROM.read(EEPROM_ADDR_BASS);
    tempSettings.treble = EEPROM.read(EEPROM_ADDR_TREBLE);
    tempSettings.switchConfig = EEPROM.read(EEPROM_ADDR_SWCONF);

    // Load audio output mode.
    tempOutputMode = EEPROM.read(EEPROM_ADDR_OUTPUT);

    // Assign only the valid audio configurations.
    if(tempSettings.volume <= VOLUME_TDA8425_MAX)
    {
        audioSettings->volume = tempSettings.volume;
        audioSettings->switchConfig = tempSettings.switchConfig;
        isValueUpdate = TRUE;
    }

    if(tempSettings.bass <= BASS_TDA8425_MAX)
    {
        audioSettings->bass = tempSettings.bass;
        isValueUpdate = TRUE;
    }

    if(tempSettings.treble <= TREBLE_TDA8425_MAX)
    {
        audioSettings->treble = tempSettings.treble;
        isValueUpdate = TRUE;
    }

    // Assign only the valid audio output configurations.
    if(tempOutputMode <= AUDIO_OUT_HEADPHONE)
    {
        *outputMode = tempOutputMode;
        isValueUpdate = TRUE;
    }

    return isValueUpdate;
}

void drawSpectrumAnalyzer()
{
    unsigned char arrayPos = 1;
    unsigned char lcdPos = 0;
    int barVal;

    while(lcdPos < ANALYZER_COLUMNS)
    {        
        // Check graph data is moving more than one LCD row.
        if(graphData[arrayPos] > 8)
        {            
            barVal = graphData[arrayPos] - 8;  
            
            lcd.setCursor(lcdPos, 0);        

            // Fill the top row of the LCD.
            if(barVal > 0)
            {     
                lcd.write((char)((barVal < 8) ? barVal : 0xFF)); 
            }
            
            // In this state bottom row of the LCD is always full.
            barVal = 0xFF;                    
        }
        else
        {
            barVal = graphData[arrayPos];

            // Check for valid graph data.
            if(barVal > 0)
            {
                barVal = (barVal < 8) ? barVal : 0xFF;
            }
            else
            {
                // Graph data is not available for the selected frequency.
                barVal = 0;
            }
        }

        // Draw the bottom raw of the LCD.
        lcd.setCursor(lcdPos, 1);

        if(barVal > 0)
        {
            lcd.write((char)barVal); 
        }

        // Move to next LCD column and frequency.
        arrayPos += 2;
        lcdPos++;
    }
}

void automaticGainControl(int *graph)
{
    unsigned char peekDataCount = 0;
    unsigned char arrayPos;
    int temp = 0;

    // Find the number of peek points across the spectrum and the maximum amplitude.
    for(arrayPos = 1; arrayPos < ((ANALYZER_COLUMNS * 2) + 1); arrayPos++)
    {
        if(graph[arrayPos] > LCD_MAX_COLUMN_HEIGHT)
        {
            peekDataCount++;
        }

        if(graph[arrayPos] > temp)
        {
            temp = graph[arrayPos];
        }
    }

    // Trim graph data based on the maximum amplitude.
    if(peekDataCount >= (ANALYZER_COLUMNS / 2))
    {
        temp = temp % LCD_MAX_COLUMN_HEIGHT;
        if(temp > 0)
        {
            for(arrayPos = 1; arrayPos < ((ANALYZER_COLUMNS * 2) + 1); arrayPos++)
            {
                graph[arrayPos] = graph[arrayPos] / temp;
            }
        }
    }
}

void updateSpectrumAnalyzer()
{
    unsigned char samplePos, tempPos;
    char tempValue;

    // Capture audio data from ADC channel 0.
    for(samplePos = 0; samplePos < ANALYZER_SAMPLES; samplePos++)
    { 
        tempValue = analogRead(A0);
        analogData[samplePos] = (char)(tempValue/4 - 128);
        imgData[samplePos] = 0;
    }

    // Perform FFT.
    fix_fft(analogData, imgData, 7, 0);

    // Extract absolute value from FFT data.
    for(samplePos = 0; samplePos < (ANALYZER_SAMPLES/2); samplePos++)
    {
        graphData[samplePos] = (int)sqrt(analogData[samplePos] * analogData[samplePos] + imgData[samplePos] * imgData[samplePos]);
    }

    for(samplePos = 0, tempPos = 0; samplePos < (ANALYZER_SAMPLES/2); samplePos++, tempPos += 2)
    {
        graphData[samplePos] = graphData[tempPos] + graphData[tempPos + 1];
    }

    // Trim graph data to avoid clipping.
    automaticGainControl(graphData);

    // Draw graph data on LCD.
    lcd.clear();    
    drawSpectrumAnalyzer();
}

void settingsMenuLoop()
{
    unsigned char temp;
    SettingsMenuState menuState = INPUT_CHANNEL;
    idleCounter = 0;
    
    // Update button states on global variables.
    btnState_Action = digitalRead(SWITCH_ACTION);
    btnState_Up = digitalRead(SWITCH_UP);
    btnState_Down = digitalRead(SWITCH_DOWN);
    btnState_Mute = digitalRead(SWITCH_MUTE);

    lcd.clear();
    lcd.print("Settings");
    displayMenuItem(&menuState, &audioSettings, &audioOutMode);
    
    while(1)
    {
        if((btnState_Action == LOW) && (digitalRead(SWITCH_ACTION) == HIGH))
        {
            // Action button press event.  
            menuState = (menuState != EXIT) ? ((SettingsMenuState)(menuState + 1)) : INPUT_CHANNEL;
            displayMenuItem(&menuState, &audioSettings, &audioOutMode);

            idleCounter = 0;
        }

        if((btnState_Mute == LOW) && (digitalRead(SWITCH_MUTE) == HIGH))
        {
            // Mute audio in sound processor and the power amplifier.
            isAudioMute = (audioSettings.switchConfig & SWITCH_MUTE_TDA8425) ? FALSE : TRUE;

            setPowerAmpMute(isAudioMute);
            muteAudio(&audioSettings, isAudioMute);        

            idleCounter = 0; 
            isLCDShowMute = TRUE;

            delay(50);
        }

        if((btnState_Up == LOW) && (digitalRead(SWITCH_UP) == HIGH))
        {
            // Up button press event.
            switch(menuState)
            {
                case INPUT_CHANNEL:
                    // Rotate to next source selection mode ranging from 0x02 to 0x07.
                    temp = audioSettings.switchConfig & 0x07;                                        
                    temp = (temp >= 0x07) ? 0x02 : (temp + 1);
                    audioSettings.switchConfig = (audioSettings.switchConfig & 0xF8) | temp;
                    setSwitchConfiguration(&audioSettings);
                    break;
                case LVL_BASS:
                    // Increase bass level and stop at the max level.
                    audioSettings.bass = (audioSettings.bass < BASS_TDA8425_MAX) ? (audioSettings.bass + 1) : BASS_TDA8425_MAX;
                    setBass(&audioSettings);
                    break;
                case LVL_TREBLE:
                    // Increase treble level and stop and the max level.
                    audioSettings.treble = (audioSettings.treble < TREBLE_TDA8425_MAX) ? (audioSettings.treble + 1) : TREBLE_TDA8425_MAX;
                    setTreble(&audioSettings);
                    break;
                case MODE_STEREO:
                    // Rotate to next channel mode ranging from 0x00 to 0x03.
                    temp = (audioSettings.switchConfig & 0x18) >> 3;
                    temp = (temp >= 0x03) ? 0x00 : (temp + 1);
                    audioSettings.switchConfig = (audioSettings.switchConfig & 0xE7) | (temp << 3);
                    setSwitchConfiguration(&audioSettings);
                    break;
                case OUTPUT_MODE:
                    // Audio output mode (speaker or headphone selection).
                    audioOutMode = (audioOutMode == AUDIO_OUT_SPEAKER) ? AUDIO_OUT_HEADPHONE : AUDIO_OUT_SPEAKER;
                    setAudioOutputMode(audioOutMode);
                    break;
                case EXIT:
                    // Save the changes and exit from the settings menu.
                    saveConfiguration(&audioSettings, &audioOutMode);
                    return;                    
            }

            // Update display with new configuration / level.
            displayMenuItem(&menuState, &audioSettings, &audioOutMode);

            idleCounter = 0;
        }

        if((btnState_Down == LOW) && (digitalRead(SWITCH_DOWN) == HIGH))
        {
            // Down button press event.
            switch(menuState)
            {
                case INPUT_CHANNEL:
                    // Rotate to next source selection mode ranging from 0x02 to 0x07.
                    temp = audioSettings.switchConfig & 0x07;                                        
                    temp = (temp == 0x02) ? 0x07 : (temp - 1);
                    audioSettings.switchConfig = (audioSettings.switchConfig & 0xF8) | temp;
                    setSwitchConfiguration(&audioSettings);
                    break;
                case LVL_BASS:
                    // Increase bass level and stop at the min level.
                    audioSettings.bass = (audioSettings.bass > BASS_TDA8425_MIN) ? (audioSettings.bass - 1) : BASS_TDA8425_MIN;
                    setBass(&audioSettings);
                    break;
                case LVL_TREBLE:
                    // Increase treble level and stop and the min level.
                    audioSettings.treble = (audioSettings.treble > TREBLE_TDA8425_MIN) ? (audioSettings.treble - 1) : TREBLE_TDA8425_MIN;
                    setTreble(&audioSettings);
                    break;
                case MODE_STEREO:
                    // Rotate to next channel mode ranging from 0x00 to 0x03.
                    temp = (audioSettings.switchConfig & 0x18) >> 3;
                    temp = (temp == 0x00) ? 0x03 : (temp - 1);
                    audioSettings.switchConfig = (audioSettings.switchConfig & 0xE7) | (temp << 3);
                    setSwitchConfiguration(&audioSettings);
                    break;
                case OUTPUT_MODE:
                    // Audio output mode (speaker or headphone selection).
                    audioOutMode = (audioOutMode == AUDIO_OUT_SPEAKER) ? AUDIO_OUT_HEADPHONE : AUDIO_OUT_SPEAKER;
                    setAudioOutputMode(audioOutMode);
                    break;
                case EXIT:
                    // Save the changes and exit from the settings menu.
                    saveConfiguration(&audioSettings, &audioOutMode);
                    return;                    
            }

            // Update display with new configuration / level.
            displayMenuItem(&menuState, &audioSettings, &audioOutMode);

            idleCounter = 0;
        }  
        
        // Update button states.
        btnState_Action = digitalRead(SWITCH_ACTION);
        btnState_Up = digitalRead(SWITCH_UP);
        btnState_Down = digitalRead(SWITCH_DOWN);  
        btnState_Mute = digitalRead(SWITCH_MUTE);     

        // Check for idle timeout.
        if(idleCounter >= IDLE_MENU_TIMEOUT)
        {
            // Idle timeout!, lets exit from the settings menu.
            saveConfiguration(&audioSettings, &audioOutMode);
            return;
        }
        else
        {
            // User is recently interacted with the system!
            idleCounter++;
            delay(50);             
        }
    }
}

void setup() 
{    
    // Initialize Arduino libraries required for I2C and LCD.
    lcd.begin(16, 2);
    lcd.clear();

    Wire.begin(); 
    
    // Configure I/O pins.
    pinMode(SWITCH_ACTION, INPUT_PULLUP);
    pinMode(SWITCH_UP, INPUT_PULLUP);
    pinMode(SWITCH_DOWN, INPUT_PULLUP);
    pinMode(SWITCH_MUTE, INPUT_PULLUP);
       
    // Initialize TDA8425 audio processor (on I2C bus).
    initSoundProcessor(&audioSettings);

    // Setup global variables.
    btnState_Action = digitalRead(SWITCH_ACTION);
    btnState_Up = digitalRead(SWITCH_UP);
    btnState_Down = digitalRead(SWITCH_DOWN);
    btnState_Mute = digitalRead(SWITCH_MUTE);

    audioOutMode  = AUDIO_OUT_SPEAKER;
    idleCounter = IDLE_TIMEOUT;
    isAudioMute = FALSE;
    isLCDShowMute = FALSE;

    // Restore last audio configuration.
    if(loadLastConfiguration(&audioSettings, &audioOutMode))
    {
        // Apply TDA8425 audio processor related configurations.
        setVolume(&audioSettings);
        setBass(&audioSettings);
        setTreble(&audioSettings);
        setSwitchConfiguration(&audioSettings);
    }

    // Release mute in TDA8425 audio processor and power amplifier.
    muteAudio(&audioSettings, isAudioMute);
    setPowerAmpMute(isAudioMute);

    // Activate last audio output.
    setAudioOutputMode(audioOutMode);

    // Define custom characters required for the spectrum analyzer.
    lcd.createChar(1, graphLine1);
    lcd.createChar(2, graphLine2);
    lcd.createChar(3, graphLine3);
    lcd.createChar(4, graphLine4);
    lcd.createChar(5, graphLine5);
    lcd.createChar(6, graphLine6);
    lcd.createChar(7, graphLine7);
}

void loop() 
{
    if((btnState_Action == LOW) && (digitalRead(SWITCH_ACTION) == HIGH))
    {
        // Action button press event.  
        settingsMenuLoop();  

        // Wait until the exit button is released by the user.
        while(!((digitalRead(SWITCH_UP) == HIGH) && (digitalRead(SWITCH_DOWN) == HIGH)))
        {
            delay(5);
        }

        // Update button state variables.
        btnState_Action = digitalRead(SWITCH_ACTION);
        btnState_Up = digitalRead(SWITCH_UP);
        btnState_Down = digitalRead(SWITCH_DOWN);
        btnState_Mute = digitalRead(SWITCH_MUTE);

        idleCounter = IDLE_TIMEOUT;                    
    }

    if((btnState_Mute == LOW) && (digitalRead(SWITCH_MUTE) == HIGH))
    {
        // Mute audio in sound processor and the power amplifier.
        isAudioMute = (audioSettings.switchConfig & SWITCH_MUTE_TDA8425) ? FALSE : TRUE;

        setPowerAmpMute(isAudioMute);
        muteAudio(&audioSettings, isAudioMute);     

        idleCounter = IDLE_TIMEOUT; 
        isLCDShowMute = TRUE;

        delay(50);
    }

    if(isAudioMute == FALSE)
    {        
        // Change volume only if the mute is released.

        if((btnState_Up == LOW) && (digitalRead(SWITCH_UP) == HIGH))
        {
            // Up button press event.        
            audioSettings.volume = (audioSettings.volume < VOLUME_TDA8425_MAX) ? (audioSettings.volume + 1) : audioSettings.volume;
            setVolume(&audioSettings);
            displayVolumeLevel(audioSettings.volume);
            idleCounter = 0;        
        }

        if((btnState_Down == LOW) && (digitalRead(SWITCH_DOWN) == HIGH))
        {
            // Down button press event.
            audioSettings.volume = (audioSettings.volume > VOLUME_TDA8425_MIN) ? (audioSettings.volume - 1) : audioSettings.volume;
            setVolume(&audioSettings);
            displayVolumeLevel(audioSettings.volume);
            idleCounter = 0;
        } 
    }
    else
    {
        // Check status of the volume up or down buttons.
        if(((btnState_Up == LOW) && (digitalRead(SWITCH_UP) == HIGH)) || ((btnState_Down == LOW) && (digitalRead(SWITCH_DOWN) == HIGH)))
        {
            // Release mute if the volume button(s) are pressed.
            isAudioMute = FALSE;

            setPowerAmpMute(isAudioMute);
            muteAudio(&audioSettings, isAudioMute);     

            // Show current volume level.
            displayVolumeLevel(audioSettings.volume);
            idleCounter = 0; 

            isLCDShowMute = FALSE;
        }
        
        // System is in mute state, and show MUTE on LCD.
        if(isLCDShowMute == TRUE)
        {
            isLCDShowMute = FALSE;
            showMute();            
        }
    }   

    // Update button states.
    btnState_Action = digitalRead(SWITCH_ACTION);
    btnState_Up = digitalRead(SWITCH_UP);
    btnState_Down = digitalRead(SWITCH_DOWN);
    btnState_Mute = digitalRead(SWITCH_MUTE);

    // To minimize the write cycles, let save the current volume level at the middle 
    // of timeout interval.
    if(idleCounter == (IDLE_TIMEOUT / 2))
    {
        saveConfiguration(&audioSettings, &audioOutMode);
    }
    
    // Place this code block at the bottom of the service loop.
    if(isAudioMute == FALSE)
    {
        // Idle detection is checked only if the output is not mute.
        if(idleCounter >= IDLE_TIMEOUT)
        {
            // System is in idle state (and display the spectrum analyzer).
            updateSpectrumAnalyzer();
            delay(5);
        }
        else
        {
            // User is recently interacted with the system!
            idleCounter++;
            delay(50);
        }
    }
    else
    {
        // System is in MUTE. 50ms delay is placed for button debouncing.
        delay(50);
    }    
}