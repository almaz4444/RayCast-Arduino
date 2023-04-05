#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>

#include "vector.h"
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
#define MapRows        20
#define MapColumns     16
#define Tile           4
#define BitTile        2  // 2 ^ BitTile = Tile
#define TileField      (Tile >> 1)
#define MapRowsTile    (MapRows << BitTile)
#define MapColumnsTile (MapColumns << BitTile)

// Ray casting
#define Fow          PI / 3
#define HalfFow      Fow / 2
#define NumRays      80
#define MaxDepth     20
#define MaxDepthTile MaxDepth >> BitTile
#define DeltaAngle   Fow / NumRays
#define Dist         NumRays / (2 * tan(HalfFow))
#define PrectCoeff   round(Width / NumRays) * Dist * 5
#define Scale        round(Width / NumRays)
#define ColorCoeff   255.0 / (MaxDepth - TileField)

// Player
const byte playerSize   = 1;
const float moveSpeed   = 0.1;
const float rotateSpeed = 0.0001;

// Font
const byte FPS_TextSize = 1;

// Colors
#define FPSColor  tft.color565(255, 255, 255)
#define SkyColor  tft.color565(135, 206, 235)
#define FlorColor tft.color565( 50,  50,  50)

const PROGMEM char* StringMap[] {
  "BBBBBBBBBBBBBBBB",
  "B  RRRR  R  R  B",
  "B R    G R RRR B",
  "B GGGG   R G   B",
  "B      GG      B",
  "B BBBB     RRRRB",
  "B B  RRR BB R RB",
  "B BR        R  B",
  "B G  G   GGGO  B",
  "B    R   R   G O",
  "R R  RBBBB R.  B",
  "R  BB       R  R",
  "R     GG RRRR GR",
  "R GRRR   B  R  R",
  "RB       B RR  R",
  "R RR GGG      BR",
  "R R     B RRR  R",
  "R BBB G B    G R",
  "R         RR G R",
  "RRRRRRRRRRRRRRRR",
};
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

Wall Map[MapRows * MapColumns];

// ________Init________
float x, y;
float angle = 1.5708;

byte wallsCount = 0;

byte fps, fps_count;
float tick, dFpsTime;

int startJoyX, startJoyY;

void setup() {
  // Init
  MapInit();

  DrawBG();
  RaysCasting(true);
}

void loop() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(90);

  pinMode(buttonPin, INPUT_PULLUP);
  startJoyY = analogRead(JoyY_Pin);
  startJoyX = analogRead(JoyX_Pin);
  fps_count++;

  // loop
  movePlayer();
  RaysCasting(false);
  // Rendering
  DrawFPS();

  // FPS counter
  uint32_t mil = millis();
  if (tick < mil - 1000) {
    tick = mil;
    fps = fps_count;
    fps_count = 0;
  }
}

Wall getWall(const byte px_x, const byte px_y) {
  if(px_x < 0 || px_x > MapRows || px_y < 0 || px_y > MapColumns) return Wall(0, 0, col_vec3(0), 0, 255, false);
  return Map[px_x * MapColumns + px_y];
}