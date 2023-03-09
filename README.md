# Ray cast arduino
Tested on a 1.8-inch TFT display with ST7789 driver running Arduino Mega 2560<br/>
***Works on SPI***

Connection:
  - Display:
    - GND        -> GND
    - CS         -> 10
    - RESET      -> 8
    - A0 (DC)    -> 9
    - SDA (MOSI) -> MOSI
    - SCK        -> SCK
    - LED (VCC)  -> 3.3V
  - Joystick:
    - VCC -> 5V
    - GND -> GND
    - VRX -> A1
    - VRY -> A0
    - SW  -> 22

Adjust the Width and Height settings to suit your display
