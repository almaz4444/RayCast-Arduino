#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "walls.h"


// ________Settings________
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

// Control
#define JoyX_Pin       A1
#define JoyY_Pin       A0
#define buttonPin      22

// General
#define Width          160
#define Height         128
#define HeightField    (Height >> 1)
#define MapRows        10
#define MapColumns     10
#define Tile           4
#define BitTile        2  // 2 ^ BitTile = Tile
#define TileField      (Tile >> 1)
#define MapRowsTile    (MapRows << BitTile)
#define MapColumnsTile (MapColumns << BitTile)

// Ray casting
const float Fow          = PI / 3;
const float HalfFow      = Fow / 2;
const byte  NumRays      = 80;
const byte  MaxDepth     = 16;
const byte  MaxDepthTile = MaxDepth >> BitTile;
const float DeltaAngle   = Fow / NumRays;
const float Dist         = NumRays / (2 * tan(HalfFow));
const byte  PrectCoeff   = round(Width / NumRays) * Dist * 5;
const byte  Scale        = round(Width / NumRays);
const float ColorCoeff   = 255.0 / (MaxDepth - TileField);

// Player
#define playerSize  1
#define moveSpeed   0.005
#define rotateSpeed 0.001

// Font
const byte FPS_TextSize = 1;

// Colors
#define FPSColor   tft.color565(255, 255, 255)
#define SkyColor   tft.color565(135, 206, 235)
#define FlorColor  tft.color565( 50,  50,  50)
#define BlackColor tft.color565(  0,   0,   0)

const char* StringMap[] {
  "BBBBBBBBBB",
  "R  RRRR  R",
  "R R    G R",
  "R GGGG   R",
  "R  .   GGR",
  "B BBBB   B",
  "B B  RRR B",
  "B BR     B",
  "B G  G   B",
  "RRRRRRRRRR",
};
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

Wall Map[MapRows * MapColumns];

// ________Init________
float x, y;
float angle = 1.5708;

byte fps, fps_count;
uint32_t tick;

float startJoyX, startJoyY;

// extern "C" char* sbrk(int incr);
// int freeRam() {
//   char top;
//   return &top - reinterpret_cast<char*>(sbrk(0));
// }

void setup() {
  // Init
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(45);
  tft.setSPISpeed(100000000000000000000000000000);
  
  pinMode(buttonPin, INPUT_PULLUP);
  startJoyY = 1.0 / analogRead(JoyY_Pin);
  startJoyX = 1.0 / analogRead(JoyX_Pin);
  
  MapInit();

  DrawBG();
  RaysCasting(true);
  
  tft.setTextSize(FPS_TextSize);
}

void loop() {
  // Serial.println(freeRam());
  fps_count++;

  movePlayer();
  
  // Rendering
  RaysCasting(false);
  DrawFPS();

  // FPS counter
  uint32_t mil = millis();
  if (tick < mil - 1000) {
    tick = mil;
    fps = fps_count;
    fps_count = 0;
    // Serial.println(fps);    
  }
}

Wall getWall(const byte px_x, const byte px_y) {
  if(px_x < 0 || px_x > MapRows || px_y < 0 || px_y > MapColumns) return Wall(true);
  return Map[px_x * MapColumns + px_y];
}