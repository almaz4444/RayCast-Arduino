#include <TFT.h>
#include <SPI.h>

// ________Settings________
// Display
#define cs   10
#define dc   9
#define rst  8

// Player
#define playerSize 2
#define speed 1

// Control
#define JoyX_Pin  A1
#define JoyY_Pin  A0
#define JoySW_Pin_mode A2
#define JoySW_Pin_option A3
#define invert_control -1

// General
#define Width  160
#define Height 128
#define Tile   16
#define TileInMiniMap 4

// Ray casting
#define Fow        PI / 3
#define HalfFow    Fow / 2
#define NumRays    32
#define MaxDepth   16 * 6
#define DeltaAngle Fow / NumRays
#define Dist       NumRays / (2 * tan(HalfFow))
#define PrectCoeff 3 * Dist * Tile
#define Scale      round(Width / NumRays)

// Font
#define FPS_TextSize 1

// Colors
byte MiniMapColor[3]        {0, 0, 255};
byte PlayerColor[3]         {50, 50, 150};
byte PlayerLookLineColor[3] {50, 50, 150};
byte FPSColor2D[3]          {255, 0, 0};
byte BGColor2D[3]          {0, 0, 0};
byte FPSColor3D[3]          {0, 0, 255};
byte SkyColor3D[3]          {150, 10, 10};
byte FlorColor3D[3]         {50, 50, 50};

// Map
String StringMap[Width / Tile] {
  "WWWWWWWW",
  "W      W",
  "WWW  WWW",
  "W      W",
  "WW  W  W",
  "W      W",
  "W WWW  W",
  "W    W W",
  "W  WW  W",
  "WWWWWWWW",
};
byte** Map = malloc(Width / Tile * StringMap[0].length() * 2);
int MiniMapOffset[2] { Width - Width / Tile * TileInMiniMap - 3, 3 };

// ________Init________
TFT TFTscreen = TFT(cs, dc, rst);

float x = Width / 2;
float y = 100;
float angle = 0;

bool Is3D = false;
bool NewOptionRender = false;
bool isButtonClicked = false;

byte wallsCount = 0;

int fps, fps_count;
float tick;

void setup() {
  // Init
  Serial.begin(9600);
  Serial.println();
  
  pinMode(JoyX_Pin,  INPUT);
  pinMode(JoyY_Pin,  INPUT);
  pinMode(JoySW_Pin_mode, INPUT);
  pinMode(JoySW_Pin_option, INPUT);
  
  TFTscreen.begin();

  // Counting the number of walls
  for(String row : StringMap) {
    for(char cell : row) {
      Map[wallsCount] = new byte[2] {0, 0};
      if(cell == 'W') wallsCount++;
    }
  }

  // Writing wall coordinates to an array
  byte index = 0;
  for(int i = 0; i < sizeof(StringMap) / sizeof(String); i++) {
    for(int j = 0; j < StringMap[i].length(); j++) {
      if(StringMap[i][j] == 'W') {
        Map[index][0] = i * Tile;
        Map[index][1] = j * Tile;
        index++;
      }
    }
  }
  DrawBG(Is3D);
}

void loop() {
  // FPS writing
  fps_count++;
  
  // loop
  RayCast();
  movePlayer();

  // Rendering
  if(Is3D) DrawMiniMap();
  else DrawMap();
  DrawFPS(true, Is3D);

  Serial.println(NewOptionRender);

  // If the button on the joystick is pressed, change the mode or option render
  if(analogRead(JoySW_Pin_mode) == 0 && !isButtonClicked) {
    Is3D = !Is3D;
    DrawBG(Is3D);
    isButtonClicked = true;
  }
  if(analogRead(JoySW_Pin_option) == 0 && !isButtonClicked) {
    NewOptionRender = !NewOptionRender;
    TFTscreen.background(0, 0, 0);
    DrawBG(Is3D);
    isButtonClicked = true;
  }
  if ((analogRead(JoySW_Pin_option) != 0 || analogRead(JoySW_Pin_mode) != 0) && isButtonClicked) {
    isButtonClicked = false;
  }
    
  // FPS counter
  if(tick < millis() / 1000 - 1) {
    tick = millis() / 1000;
    fps = fps_count;
    fps_count = 0;
  }
}