#include <TFT.h>
#include <SPI.h>

// ________Settings________
// Display
#define cs   10
#define dc   9
#define rst  8

// Player
#define playerSize 2
#define speed 0.5

// Control
#define JoyX_Pin  A1
#define JoyY_Pin  A0
#define JoySW_Pin_mode A2
#define invert_control -1

// General
#define Width  160
#define Height 128
#define Tile   4
#define TileInMiniMap 4

// Ray casting
#define Fow        PI / 3
#define HalfFow    Fow / 2
#define NumRays    32
#define MaxDepth   Tile * 20
#define DeltaAngle Fow / NumRays
#define Dist       NumRays / (2 * tan(HalfFow))
#define PrectCoeff 3 * Dist * Tile
#define Scale      round(Width / NumRays)

// Font
#define FPS_TextSize 1

// Colors
byte MiniMapColor[3]        {0, 0, 255};
byte PlayerColor[3]         {0, 255, 0};
byte PlayerLookLineColor[3] {50, 50, 150};
byte FPSColor2D[3]          {255, 0, 0};
byte BGColor2D[3]           {0, 0, 0};
byte FPSColor3D[3]          {0, 0, 255};
byte SkyColor3D[3]          {150, 30, 30};
byte FlorColor3D[3]         {50, 50, 50};

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

// String StringMap[MapRows] {
//   "BBBBBBBBBBBBBBBB",
//   "B  RRRR  R  R  B",
//   "B R    G R RRR B",
//   "B GGGG   R G   B",
//   "B      GG      B",
//   "B BBBB     RRRRB",
//   "B B  RRR BB R RB",
//   "B BR        R  B",
//   "B G  GG GGGGR  B",
//   "B    R   R   B B",
//   "R R  RBBBB R   R",
//   "R  BB   W   R  R",
//   "R     GG RRRR GR",
//   "R GRRR   B  R  R",
//   "RB       B RR  R",
//   "R RR GGG      BR",
//   "R R     B RRR  R",
//   "R BBB G B    G R",
//   "R         RR G R",
//   "RRRRRRRRRRRRRRRR",
// };
String StringMap[MapRows] {
  "BBBBBBBBBBBBBBBB",
  "B              B",
  "B W            B",
  "B              B",
  "B              B",
  "B              B",
  "B              B",
  "B              B",
  "B     W        B",
  "B              B",
  "R              R",
  "R         W    R",
  "R              R",
  "R              R",
  "R  W           R",
  "R              R",
  "R             WR",
  "R              R",
  "R              R",
  "RRRRRRRRRRRRRRRR",
};

Brick* Map = new Brick[MapRows * StringMap[0].length()];
int MiniMapOffset[2] { Width - MapRows * TileInMiniMap - 3, 3 };

// ________Init________
TFT TFTscreen = TFT(cs, dc, rst);

float x = Tile * MapRows / 2 + Tile * 0.5;
float y = Tile * MapRows / 2 + Tile * 2;
float z = 0;
float angle = -80;

bool Is3D = true;
bool isButtonClicked = false;

byte wallsCount = 0;

int fps, fps_count;
float tick;

void setup() {
  // Init
  Serial.begin(115200);
  Serial.println();
  
  pinMode(JoyX_Pin,  INPUT);
  pinMode(JoyY_Pin,  INPUT);
  pinMode(JoySW_Pin_mode, INPUT);
  
  TFTscreen.begin();

  MapInit();
  
  DrawBG(Is3D);
}

void loop() {
  // FPS writing
  fps_count++;
  
  // loop
  MapUpdate();
  RayCast();
  movePlayer();

  // Rendering
  if(!Is3D) {
    DrawMap();
  }
  DrawFPS(true);

  Serial.println(analogRead(JoySW_Pin_mode));
  // If the button on the joystick is pressed, change the mode or option render
  if(analogRead(JoySW_Pin_mode) == 0 && !isButtonClicked) {
    Is3D = !Is3D;
    DrawBG(Is3D);
    isButtonClicked = true;
  }
  if (analogRead(JoySW_Pin_mode) != 0 && isButtonClicked) {
    isButtonClicked = false;
  }
    
  // FPS counter
  if(tick < millis() / 1000 - 1) {
    tick = millis() / 1000;
    fps = fps_count;
    fps_count = 0;
  }
}