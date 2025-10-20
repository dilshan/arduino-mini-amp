# Arduino Mini Amplifier

The **Arduino Mini Amplifier** is a complete, open-source audio amplifier system that combines vintage analog audio processing with modern embedded control. It features Bluetooth and line-in audio inputs, tone adjustment, multiple stereo enhancement modes, and a 16-channel real-time audio spectrum analyzer displayed on an LCD.

At its core, this design integrates the Philips TDA8425 analog audio processor - introduced in the late 1980s and known for its high-quality tone control system - with the Yamaha YDA138, a compact Class-D digital power amplifier that delivers up to 2×10 W RMS of clear, low-distortion stereo output.

An ATmega328P microcontroller platform (Arduino Nano) manages all audio controls, user interface elements, and real-time visualizations.

## Features

- Based on Arduino Nano (ATmega328P)
- Bluetooth and Line-in audio input support
- 16-channel spectrum analyzer on 16×2 LCD
- Tone control (bass, treble) and mute function
- Speaker / Headphone output modes
- Stereo modes: Pseudo, Spatial, Linear, and Forced Mono
- Software control over all audio parameters via I2C

## Firmware

The firmware is written in C++ using the Arduino framework and developed under PlatformIO.

To build and upload the firmware:

1. Install PlatformIO (as a standalone tool or VS Code extension).
2. Clone this repository and open it in PlatformIO.
3. Connect your Arduino Nano (or compatible ATmega328P board) to your computer via USB.
4. From the PlatformIO toolbar or terminal, run:

```
pio run --target upload
```

This will compile and flash the firmware to the board automatically.

The firmware automatically initializes the audio processor, LCD, and input controls at startup. All adjustable parameters—such as tone, volume, and stereo mode - are stored in built-in EEPROM and restored on each power cycle.

## Hardware Design

This amplifier unit of the following functional modules:
- **Audio Processor** – TDA8425-based tone and input control
- **Spectrum Analyzer** – 16-band with seperate NE5532 op-amp buffer
- **Power Amplifier** – Yamaha YDA138 Class-D stage (2 × 10 W RMS)
- **Bluetooth Source** – AC6939 module with 78L05 regulator
- **Control Interface** – Arduino Nano, LCD, and tactile switches
- **Power Supply** – 12V DC input with multiple regulated rails

## License

This is an open hardware project and all its design files, firmware source code, documentation, and other resource files are available at this project source repository. The content of this project is distributed under the terms of the following licenses:

- Hardware License: CERN-OHL-W 2.0
- Software License: MIT
- Documentation License: CC BY 4.0
