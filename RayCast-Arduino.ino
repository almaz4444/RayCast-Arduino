#include <TFT.h>
#include <SPI.h>


// ________Settings________
// Display
#define cs 10
#define dc 9
#define rst 8

// Player
#define playerSize 2
#define moveSpeed 0.5
#define rotateSpeed 1 / 1200
#define dTimeMovement 1

// Control
#define JoyX_Pin A1
#define JoyY_Pin A0
#define buttonPin 22
#define invert_control -1

// General
#define Width 160
#define Height 128
#define Tile 4
#define TileInMiniMap 4

// Ray casting
#define Fow PI / 3
#define HalfFow Fow / 2
#define NumRays 32
#define MaxDepth Tile * 5
#define DeltaAngle Fow / NumRays
#define Dist NumRays / (2 * tan(HalfFow))
#define PrectCoeff 3 * Dist* Tile
#define Scale round(Width / NumRays)

// Font
#define FPS_TextSize 1

// Colors
const byte MiniMapColor[3]{ 0, 0, 255 };
const byte PlayerColor[3]{ 0, 255, 0 };
const byte PlayerLookLineColor[3]{ 50, 50, 150 };
const byte FPSColor[3]{ 255, 255, 255 };
const byte BGColor2D[3]{ 0, 0, 0 };
const byte SkyColor3D[3]{ 150, 30, 30 };
const byte FlorColor3D[3]{ 50, 50, 50 };

// Map
class Brick {
public:
  Brick() {}
  Brick(byte nX, byte nY, byte nColor, byte n_dTime = 0, char nDirection = ' ') {
    x = nX;
    y = nY;
    color = nColor;
    dTime = n_dTime;
    direction = nDirection;
  }
  byte x, y, color, dTime;
  char direction;
};
#define MapRows 20

const String StringMap[MapRows]{
  "BBBBBBBBBBBBBBBB",
  "B  RRRR  R  R  B",
  "B R    G R RRR B",
  "B GGGG   R G   B",
  "B      GG      B",
  "B BBBB     RRRRB",
  "B B  RRR BB R RB",
  "B BR        R  B",
  "B G  G   GGGR  B",
  "B    R   R   B B",
  "R R  RBBBB R   R",
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
// String StringMap[MapRows] {
//   "BBBBBBBBBBBBBBBB",
//   "B              B",
//   "B W            B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B              B",
//   "B     W        B",
//   "B              B",
//   "R              R",
//   "R         W    R",
//   "R              R",
//   "R              R",
//   "R  W           R",
//   "R              R",
//   "R             WR",
//   "R              R",
//   "R              R",
//   "RRRRRRRRRRRRRRRR",
// };

Brick* Map = malloc(MapRows * StringMap[0].length() * sizeof(Brick));

// ________Init________
TFT TFTscreen = TFT(cs, dc, rst);

float x = Tile * MapRows / 2 + Tile * 0.5;
float y = Tile * MapRows / 2 + Tile * 2;
// float z = 0;
float angle = -80;

boolean Is3D = true;
boolean isButtonPinClicked;

byte wallsCount = 0;

byte fps, fps_count;
float tick, dFpsTime;

// Скетч использует 17014 байт (6%) памяти устройства. Всего доступно 253952 байт.
// Глобальные переменные используют 978 байт (11%) динамической памяти, оставляя 7214 байт для локальных переменных. Максимум: 8192 байт.

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
  // FPS writing
  fps_count++;

  // loop
  // MapUpdate();
  RayCast(false);
  movePlayer();
  dFpsTime = millis();

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