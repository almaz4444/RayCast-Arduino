#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <GyverButton.h>

#include "wall.h"


// ________Macros________
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr += (x);\
  if (flag)


// ________Settings________
#define TFT_CS 6
#define TFT_RST 7
#define TFT_DC 8

// Control
#define JoyX_Pin       A0
#define JoyY_Pin       A1
#define JoyButtonPin   22
#define UpButtonPin   28
#define DownButtonPin 34

// General
#define Width           160
#define Height          128
#define MaxGameWidth    128
byte GameWidth  =       128;
byte GameHeight =       128;
#define GameHeightField (GameHeight >> 1)
#define MapRows         10
#define MapColumns      10
#define Tile            4
#define BitTile         2  // 2 ^ BitTile = Tile
#define TileField       (Tile >> 1)
#define MapRowsTile     (MapRows << BitTile)
#define MapColumnsTile  (MapColumns << BitTile)

// Ray casting
const float Fow          = PI / 3;
const float HalfFow      = Fow / 2;
const byte  MaxDepth     = 16;
const byte  MaxDepthTile = MaxDepth >> BitTile;

byte  NumRays    = GameWidth;
float DeltaAngle;
float Dist;
byte  PrectCoeff;
byte  Scale;

// Player
#define playerSize  1
#define moveSpeed   0.005
#define rotateSpeed 0.001

// Font
const byte UI_TextSize = 1;

// Colors
#define NormalColor   tft.color565(255, 255, 255)
#define SelectedColor tft.color565(  0, 255,   0)
#define SkyColor      tft.color565(135, 206, 235)
#define FlorColor     tft.color565( 50,  50,  50)
#define BlackColor    tft.color565(  0,   0,   0)

// UI
const byte FPS_TextPos[2]     {135,   5};
const byte NumRays_TextPos[2] {135,  35};
const byte Width_TextPos[2]   {135,  65};
const byte Height_TextPos[2]  {135,  95};

// Buttons
GButton JoyButton(JoyButtonPin);
GButton UpButton(UpButtonPin);
GButton DownButton(DownButtonPin);

// Map
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
Wall Map[MapRows * MapColumns];

// Dispaly
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Init
float x, y, angle;

unsigned int fps, fps_count;
uint32_t tick;

float startJoyX, startJoyY;

byte changeHandle;

// extern "C" char* sbrk(int incr);
// int freeRam() {
//   char top;
//   return &top - reinterpret_cast<char*>(sbrk(0));
// }

uint16_t oldWalls[Width][2];
  

void setup() {
  pinMode(JoyButtonPin, INPUT_PULLUP);
  pinMode(UpButtonPin, INPUT_PULLUP);
  pinMode(DownButtonPin, INPUT_PULLUP);
  // Init
  Serial.begin(115200);
  // Wire.begin();
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(45);
  tft.setSPISpeed(100000000000000000000000000000);
  
  startJoyY = 1.0 / analogRead(JoyY_Pin);
  startJoyX = 1.0 / analogRead(JoyX_Pin);
  
  MapInit();
  UpdateCastSettings();
}

void loop() {
  // Serial.println(freeRam());
  fps_count++;

  ButtonsTick();
  InputHandle();
  
  // Rendering
  RaysCasting();
  DrawUI();

  // FPS counter
  const uint64_t mil = millis();
  if (tick < mil - 1000) {
    tick = mil;
    fps = fps_count;
    fps_count = 0;
  }
}

Wall getWall(const byte px_x, const byte px_y) {
  if(px_x < 0 || px_x > MapRows || px_y < 0 || px_y > MapColumns) return Wall(true);
  return Map[px_x * MapColumns + px_y];
}