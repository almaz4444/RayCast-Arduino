#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <GyverButton.h>

#include "wall.h"

/* ________Define________ */
// #define DEBUG_ENABLE
// #define RAM_GETTER_ENABLE

// Macros
#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

#ifdef RAM_GETTER_ENABLE
extern "C" char* sbrk(int incr);
int freeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

/*  ________Settings________ */
#define TFT_CS 6
#define TFT_RST 7
#define TFT_DC 8

// Control
#define JoyX_Pin       A0
#define JoyY_Pin       A1
#define JoyButtonPin   22
#define UpButtonPin   28
#define DownButtonPin 34

// Dispaly
#define DisplayWidth    160
#define DisplayHeight   128

#define MaxGameWidth    DisplayHeight
#define MinGameWidth    4
#define MaxGameHeight   DisplayHeight
#define MinGameHeight   4

byte    GameWidth  =    MaxGameWidth;
byte    GameHeight =    DisplayHeight;

#define GameHeightField (GameHeight >> 1)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Map
#define Tile            4
#define BitTile         2  // 2 ^ BitTile = Tile
#define TileField       (Tile >> 1)

#define MapRows         10
#define MapColumns      10
#define MapRowsTile     (MapRows << BitTile)
#define MapColumnsTile  (MapColumns << BitTile)

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

// Ray casting
#define MaxNumRays GameWidth
#define MinNumRays 1

const float Fow          = PI / 3;
const float HalfFow      = Fow / 2;
const byte  MaxDepth     = 16;
const byte  MaxDepthTile = MaxDepth >> BitTile;

byte  NumRays    = GameWidth;
float DeltaAngle, Dist;
byte  PrectCoeff, Scale;

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

// UI positions
const byte FPS_TextPos[2]     {135,   5};
const byte NumRays_TextPos[2] {135,  35};
const byte Width_TextPos[2]   {135,  65};
const byte Height_TextPos[2]  {135,  95};

// Buttons
GButton JoyButton(JoyButtonPin);
GButton UpButton(UpButtonPin);
GButton DownButton(DownButtonPin);
#define ButtonsDebounce 150


/*  ________Init________ */
// Player
float x, y, angle;

// FPS
unsigned int fps, fps_count;
uint32_t tick;

// Joystick
float startJoyDeviderX, startJoyDeviderY;

// Other
byte changeableGameSetting;
uint16_t oldWalls[MaxGameWidth][2];

void setup() {
  // Init
  #ifdef DEBUG_ENABLE
  Serial.begin(115200);
  #endif

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(45);
  tft.setSPISpeed(100000000000000000000000000000); // If you are having problems with the display, try removing this line
  
  startJoyDeviderX = 1.0 / analogRead(JoyX_Pin);
  startJoyDeviderY = 1.0 / analogRead(JoyY_Pin);
  
  MapInit();
  UpdateCastSettings();
  SetSettingsButton();
}

void loop() {
  // DEBUG(freeRam());
  fps_count++;

  // Update
  Tick();
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

const Wall errorWall = Wall(true);
Wall getWall(const byte px_x, const byte px_y) {
  return (px_x < 0 || px_x > MapRows || px_y < 0 || px_y > MapColumns)? errorWall : Map[px_x * MapColumns + px_y];
}