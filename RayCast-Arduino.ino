#include <TFT.h>
#include <SPI.h>
#include "Textures.h"


// ________Settings________
// General
#define Width 160
#define Height 128
#define MapRows 20
#define MapColumns 16
#define Tile 4
#define BitTile 2       // 2 ^ BitTile = Tile
#define TileInMiniMap round(Width / MapRows)
#define BitTileInMiniMap 3    // 2 ^ BitTileInMiniMap = TileInMiniMap

// Display
#define cs 10
#define dc 9
#define rst 8

// Ray casting
#define Fow PI / 3
#define HalfFow Fow / 2
#define NumRays 80
#define MaxDepth Tile * 10
#define DeltaAngle Fow / NumRays
#define Dist NumRays / (2 * tan(HalfFow))
#define PrectCoeff round(Width / NumRays) * Dist * Tile
#define Scale round(Width / NumRays)

// Player
#define playerSize 1
#define moveSpeed 0.3
#define rotateSpeed 1 / 2500.0

// Control
#define JoyX_Pin A1
#define JoyY_Pin A0
#define buttonPin 22

// Font
#define FPS_TextSize 1

// Colors
const byte MiniMapColor[3]        { 0, 0, 255 };
const byte PlayerColor[3]         { 0, 255, 0 };
const byte PlayerLookLineColor[3] { 50, 50, 150 };
const byte FPSColor[3]            { 255, 255, 255 };
const byte BGColor2D[3]           { 0, 0, 0 };
const byte SkyColor3D[3]          { 235, 206, 135 };
const byte FlorColor3D[3]         { 50, 50, 50 };

// Map
class Brick {
public:
  Brick() {}
  Brick(byte nX, byte nY, byte nColor) {
    x = nX;
    y = nY;
    color = nColor;
  }
  byte x, y, color;
};

const String StringMap[] {
  "BBBBBBBBBBBBBBBB",
  "B  RRRR  R  R  B",
  "B R    G R RRR B",
  "B GGGG   R G   B",
  "B      GG      B",
  "B BBBB     RRRRB",
  "B B  RRR BB R RB",
  "B BR        R  B",
  "B G  G   GGGR  B",
  "B    R   R   G B",
  "R R  RBBBB R.  G",
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
// const String StringMap[] {
//   "BBBBBBBBBBBBBBBB",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B     . W      B",
//   "R              G",
//   "R              R",
//   "R              R",
//   "R              R",
//   "R              R",
//   "R              R",
//   "R              R",
//   "R              R",
//   "R              R",
//   "RRRRRRRRRRRRRRRR",
// };

Brick* Map = malloc(MapRows * MapColumns * sizeof(Brick));

// ________Init________
const TFT TFTscreen = TFT(cs, dc, rst);

float x, y, z;
float angle = 1.5708;

bool Is3D = true;
bool isButtonPinClicked;

byte wallsCount = 0;

byte fps, fps_count;
float tick, dFpsTime;

void setup() {
  // Init
  Serial.begin(115200);
  Serial.println();

  pinMode(buttonPin, INPUT_PULLUP);

  TFTscreen.begin();

  MapInit();

  DrawBG();
}

void loop() {
  for(;;) {
    // FPS writing
    fps_count++;

    // loop
    // MapUpdate();
    RayCast(false);
    movePlayer();

    // Rendering
    if (!Is3D) DrawMap(false);
    DrawFPS(Is3D);

    // If the buttonPin on the joystick is pressed, change the mode or option render
    boolean buttonState = PINA & 0b00000001;
    if (buttonState) isButtonPinClicked = false;
    else {
      if (isButtonPinClicked) {
        Is3D = !Is3D;
        isButtonPinClicked = false;
        DrawBG();
        if (!Is3D) DrawMap(true);
        else RayCast(true);
        DrawFPS(true);
      } else isButtonPinClicked = true;
    }

    // FPS counter
    if (tick < millis() - 1000) {
      tick = millis();
      fps = fps_count;
      fps_count = 0;
    }
  }
}