[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&weight=600&size=40&pause=1000&color=F7DC00&center=true&vCenter=true&width=1000&height=101&lines=Arduino+raycasting+game)](https://git.io/typing-svg)

# Arduino Raycasting Game

_This repository has been rewritten on `PlatformIO` to speed up work. You can claim it [here](https://github.com/almaz4444/RayCast-Arduino/tree/RayCast-Arduino-PlatformIO)_

This is a raycasting game implementation for the Arduino boards. The game consists of a 2D map with walls and player movement using a joystick. The game has a raycasting engine to render the 3D environment based on the 2D map, and a simple UI with an FPS counter and other settings.

Tested on `160x128px` TFT display with ``ST7789`` driver under `Arduino Mega 2560 (~20fps)` and `Arduino due (~64fps)` in standard mode.

## Dependencies

The following libraries are required to compile and run this project:

- [Adafruit_ST7735](https://github.com/adafruit/Adafruit-ST7735-Library): For managing the TFT color display.
- [GyverButton](https://github.com/GyverLibs/GyverButton): For managing button debounce and handling user input.
- SPI: to communicate the board with the display via the SPI interface. This library is part of the Arduino IDE.

## Features

- Raycasting engine for 3D rendering
- Simple user interface with fps counter, settings for game window size and number of beams
- Control joystick and buttons to move the player and select settings
- Map with walls and player starting position

## Usage

1. Download this repository
2. Make sure you have the Arduino IDE installed.
3. Install the required libraries (`Adafruit_ST7735` and `GyverButton`) using the links above.
4. Open the Arduino IDE and load the code into the editor.
5. Connect your Arduino board to your computer.
6. Select your board type and port in the Arduino IDE.
7. Upload the code to your board by clicking the `"Upload"` button.

## Connection

- Display:
  - ``GND``        -> ``GND``
  - ``CS``         -> ``6``
  - ``RESET``      -> ``7``
  - ``A0 (DC)``    -> ``8``
  - ``SDA (MOSI)`` -> ``MOSI``
  - ``SCK``        -> ``SCK``
  - ``LED (VCC)``  -> ``3.3V``
- Joystick:
  - ``VCC`` -> ``VCC``
  - ``GND`` -> ``GND``
  - ``VRX`` -> ``A0``
  - ``VRY`` -> ``A1``
- Mechanical button 1:
  - ``Pin1`` -> ``GND``
  - ``Pin2`` -> ``22``
- Mechanical button 2:
  - ``Pin1`` -> ``GND``
  - ``Pin2`` -> ``30``
- Mechanical button 3:
  - ``Pin1`` -> ``GND``
  - ``Pin2`` -> ``34``
  
Change button pins if necessary.

## Building a map

Keys:
- ``"."`` -> player (only 1 per card)
- ``"R"`` -> red wall
- ``"G"`` -> green wall
- ``"B"`` -> blue wall
- ``" "`` -> void
Change MapRows and MapColumns to your values.

## What's ready
- Raycasting technology.
- Good fps.
- Compatibility (AVR and ARM).
- Editable map.

## What to do
- Texturing walls.

<pre>
 ____                                                       __      
/\  _`\                                                    /\ \__   
\ \ \L\ \      __      __  __      ___      __       ____  \ \ ,_\  
 \ \ ,  /    /'__`\   /\ \/\ \    /'___\  /'__`\    /',__\  \ \ \/  
  \ \ \\ \  /\ \L\.\_ \ \ \_\ \  /\ \__/ /\ \L\.\_ /\__, `\  \ \ \_ 
   \ \_\ \_\\ \__/.\_\ \/`____ \ \ \____\\ \__/.\_\\/\____/   \ \__\
    \/_/\/ / \/__/\/_/  `/___/> \ \/____/ \/__/\/_/ \/___/     \/__/
                           /\___/                                   
                           \/__/                                    
</pre>
