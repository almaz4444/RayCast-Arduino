#include <TFT.h>
#include <SPI.h>

#define cs   10
#define dc   9
#define rst  8

#define invert_control -1
#define playerSize 1
#define speed 1

#define Width  160
#define Height 128
#define Tile   16
#define TileInMiniMap 3

#define Fow PI / 3
#define HalfFow Fow / 2
#define NumRays 32
#define MaxDepth 16 * 6
#define DeltaAngle Fow / NumRays
#define Dist NumRays / (2 * tan(HalfFow))
#define PrectCoeff 3 * Dist * Tile
#define Scale round(Width / NumRays)

byte MiniMapColor[3] {0, 0, 255};
byte PlayerColor[3] {50, 50, 50};
byte PlayerLookLineColor[3] {50, 50, 50};
byte FPSColor[3] {0, 0, 255};
byte SkyColor[3] {150, 10, 10};
byte FlorColor[3] {50, 50, 50};

TFT TFTscreen = TFT(cs, dc, rst);

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
int MiniMapOffset[2] { Width - StringMap[0].length() * TileInMiniMap - 10, Height / Tile * TileInMiniMap - 10 };

float x = Width / 2;
float y = 100;
float oldX = x;
float oldY = y;
float angle = 0;

byte wallsCount = 0;

int fps, fps_count, oldFps;
float tick;

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  TFTscreen.begin();

  for(String row : StringMap) {
    for(char cell : row) {
      Map[wallsCount] = new byte[2] {0, 0};
      if(cell == 'W') wallsCount++;
    }
  }

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
}

void loop() {
  fps_count++;
  if(oldFps != fps) {
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(((String)oldFps).c_str(), 5, 5);
    TFTscreen.noStroke();
    oldFps = fps;
  }
  RayCast();
  movePlayer();
  
  if(tick < millis() / 1000 - 1) {
    tick = millis() / 1000;
    fps = fps_count;
    fps_count = 0;
  }
}

void movePlayer() {
  float paddleX = (analogRead(A0) - 1064 / 2) / 320;
  float paddleY = (analogRead(A1) - 1064 / 2) / 320;

  if(paddleY > 0) {
    x += -speed * cos(angle);
    y += -speed * sin(angle);
  } else if(paddleY < 0) {
    x += speed * cos(angle);
    y += speed * sin(angle);
  }
  angle -= paddleX / 10;

  if(oldX != x || oldY != y) {
    TFTscreen.fill(0, 0, 0);
    TFTscreen.circle(MiniMapOffset[0] + oldX / Tile * TileInMiniMap, MiniMapOffset[1] + oldY / Tile * TileInMiniMap, playerSize);
  }
  TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
  TFTscreen.circle(MiniMapOffset[0] + x / Tile * TileInMiniMap, MiniMapOffset[1] + y / Tile * TileInMiniMap, playerSize);
  
  oldX = x;
  oldY = y;
}