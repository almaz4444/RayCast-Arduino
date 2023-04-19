# Ray cast arduino

What is this
------
Raycasting technology implemented on arduino.<br>
Tested on `160x128px` TFT display with ``ST7789`` driver under `Arduino Mega 2560 (~18fps)` and `Arduino due (~60fps)`.

What's ready
------
- Raycasting technology
- Good fps
- Compatibility (AVR and ARM)
- Editable map

What to do
------
- Texturing walls
- Speed up rendering

Connection
------
- Display:
  - GND        -> GND
  - CS         -> 10
  - RESET      -> 8
  - A0 (DC)    -> 9
  - SDA (MOSI) -> MOSI
  - SCK        -> SCK
  - LED (VCC)  -> 3.3V
- Joystick:
  - VCC -> VCC
  - GND -> GND
  - VRX -> A1
  - VRY -> A0
  - SW  -> 22

Map
------
- "." -> player (only 1 per card)
- "R" -> red wall
- "G" -> green wall
- "B" -> blue wall
- " " -> void
