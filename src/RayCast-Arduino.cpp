#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <GyverButton.h>

#include "wall.h"
#include "textures.h"

/* ________Define________ */
#define DEBUG_ENABLE
// #define RAM_GETTER_ENABLE

// Macros
#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

#ifdef RAM_GETTER_ENABLE
extern "C" char *sbrk(int incr);
int freeRam()
{
  char top;
  return &top - reinterpret_cast<char *>(sbrk(0));
}
#endif

/*  ________Settings________ */
#define TFT_CS 6
#define TFT_RST 7
#define TFT_DC 8

// Control
#define JoyX_Pin A0
#define JoyY_Pin A1
#define SelectButtonPin 22
#define UpButtonPin 28
#define DownButtonPin 34

// Dispaly
#define DisplayWidth 160
#define DisplayHeight 128

#define MaxGameWidth DisplayHeight
#define MinGameWidth 4
#define MaxGameHeight DisplayHeight
#define MinGameHeight 4

byte GameWidth = MaxGameWidth;
byte GameHeight = DisplayHeight;
#define GameHeightField (GameHeight >> 1)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Map
#define Tile 4
#define BitTile 2 // 2 ^ BitTile = Tile
const byte TileField = (Tile >> 1);

#define MapRows 10
#define MapColumns 10
const byte MapRowsTile = (MapRows << BitTile);
const byte MapColumnsTile = (MapColumns << BitTile);

const char *StringMap[]{
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

const float Fow = PI / 3;
const float HalfFow = Fow / 2;
const byte MaxDepth = 16;
const byte MaxDepthTile = MaxDepth >> BitTile;

byte NumRays = GameWidth;
float DeltaAngle, Dist;
byte PrectCoeff, Scale;

// Player
#define playerSize 1
#define moveSpeed 0.005
#define rotateSpeed 0.001

// Font
#define UI_TextSize 1

// Colors
#define NormalColor tft.color565(255, 255, 255)
#define SelectedColor tft.color565(0, 255, 0)
#define SkyColor tft.color565(135, 206, 235)
#define FlorColor tft.color565(50, 50, 50)
#define BlackColor tft.color565(0, 0, 0)

// UI positions
const byte FPS_TextPos[2]{135, 5};
const byte NumRays_TextPos[2]{135, 35};
const byte Width_TextPos[2]{135, 65};
const byte Height_TextPos[2]{135, 95};

// Buttons
GButton SelectButton(SelectButtonPin);
GButton UpButton(UpButtonPin);
GButton DownButton(DownButtonPin);
#define ButtonsDebounce 200

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
bool isFullScreenGame = false;
uint16_t oldWalls[DisplayWidth][2];

/* ________MAP________ */
const Wall errorWall = Wall(true);
Wall getWall(const byte px_x, const byte px_y)
{
  return (px_x < 0 || px_x > MapRows || px_y < 0 || px_y > MapColumns) ? errorWall : Map[px_x * MapColumns + px_y];
}

void MapInit()
{
  // Counting the number of walls
  for (byte i = 0; i < MapRows; i++)
  {
    for (byte j = 0; j < MapColumns; j++)
    {
      switch (pgm_read_byte(&StringMap[i][j]))
      {
      case 'R':
        Map[i * MapColumns + j] = Wall(tft.color565(255, 0, 0), WallTextureR);
        break;
      case 'G':
        Map[i * MapColumns + j] = Wall(tft.color565(0, 255, 0), WallTextureG);
        break;
      case 'B':
        Map[i * MapColumns + j] = Wall(tft.color565(0, 0, 255), WallTextureB);
        break;
      case 'T':
        Map[i * MapColumns + j] = Wall(tft.color565(255, 0, 0), WallTextureRGB);
        break;
      case '.':
        x = (i << BitTile) + TileField;
        y = (j << BitTile) + TileField;
        break;
      }
    }
  }
}

/* ________DRAW________ */
void DrawBG()
{
  tft.fillScreen(BlackColor);
  tft.fillRect(0, 0, GameWidth, GameHeightField, SkyColor);
  tft.fillRect(0, GameHeightField, GameWidth, GameHeightField, FlorColor);
}

uint16_t oldFPS;
size_t fpsSize;
void DrawFPS()
{
  if (!fpsSize || isFullScreenGame)
  {
    tft.setCursor(FPS_TextPos[0], FPS_TextPos[1]);
    tft.setTextSize(UI_TextSize);
    tft.setTextColor(NormalColor);
    fpsSize = tft.println("FPS:") << 1;
  }
  if (oldFPS != fps || isFullScreenGame)
  {
    tft.setTextSize(UI_TextSize);
    tft.setCursor(FPS_TextPos[0], FPS_TextPos[1] + fpsSize);

    tft.setTextColor(BlackColor);
    tft.print(oldFPS);
    tft.setCursor(FPS_TextPos[0], FPS_TextPos[1] + fpsSize);

    tft.setTextColor(NormalColor);
    tft.print(fps);

    oldFPS = fps;
  }
}

size_t numRaysSize;
void DrawNumRaysCount()
{
  tft.setTextSize(UI_TextSize);
  (changeableGameSetting == 0) ? tft.setTextColor(SelectedColor) : tft.setTextColor(NormalColor);
  tft.setCursor(NumRays_TextPos[0], NumRays_TextPos[1] + numRaysSize);
  if (!numRaysSize)
  {
    numRaysSize = tft.println("RC:") << 1;
    tft.setCursor(NumRays_TextPos[0], NumRays_TextPos[1] + numRaysSize);
  }
  tft.print(NumRays);
}

size_t widthTextSize;
void DrawGameWidth()
{
  tft.setTextSize(UI_TextSize);
  (changeableGameSetting == 1) ? tft.setTextColor(SelectedColor) : tft.setTextColor(NormalColor);
  tft.setCursor(Width_TextPos[0], Width_TextPos[1] + widthTextSize);
  if (!widthTextSize)
  {
    widthTextSize = tft.println("W:") << 1;
    tft.setCursor(Width_TextPos[0], Width_TextPos[1] + widthTextSize);
  }
  tft.print(GameWidth);
}

size_t heightTextSize;
void DrawGameHeight()
{
  tft.setTextSize(UI_TextSize);
  (changeableGameSetting == 2) ? tft.setTextColor(SelectedColor) : tft.setTextColor(NormalColor);
  tft.setCursor(Height_TextPos[0], Height_TextPos[1] + heightTextSize);
  if (!heightTextSize)
  {
    heightTextSize = tft.println("H:") << 1;
    tft.setCursor(Height_TextPos[0], Height_TextPos[1] + heightTextSize);
  }
  tft.print(GameHeight);
}

void DrawCastWalls(const byte castedWalls[MaxGameWidth][3])
{
  for (byte ray = 0; ray < NumRays; ray++)
  {
    const byte proect_height = castedWalls[ray][0];
    const uint16_t texture = (castedWalls[ray][1] == 0 && castedWalls[ray][2] == 0) ? BlackColor : getWall(castedWalls[ray][1], castedWalls[ray][2]).color;
    const byte oldWallProjectHeight = oldWalls[ray][0];
    const byte oldWallProjectHeightField = (oldWallProjectHeight >> 1);
    const byte proectHeightField = (proect_height >> 1);
    const byte brickY = GameHeightField - proectHeightField;
    const byte differenceWallsHeight = oldWallProjectHeightField - proectHeightField;
    const byte brickX = ray * Scale;

    // fill the voids
    if (oldWallProjectHeight > proect_height)
    {
      const byte oldBrickYTop = GameHeightField - oldWallProjectHeightField;
      tft.fillRect(brickX, oldBrickYTop, Scale, differenceWallsHeight, SkyColor);
      tft.fillRect(brickX, oldBrickYTop + oldWallProjectHeightField + proectHeightField, Scale, differenceWallsHeight + 1, FlorColor);
    }

    // Render of new walls
    if (oldWalls[ray][1] != texture)
    {
      tft.fillRect(brickX, brickY, Scale, proect_height, texture);
    }
    else if (oldWallProjectHeight < proect_height)
    {
      const byte oldBrickY = proectHeightField - oldWallProjectHeightField;
      tft.fillRect(brickX, brickY, Scale, oldBrickY, texture);
      tft.fillRect(brickX, GameHeightField + oldWallProjectHeightField, Scale, oldBrickY, texture);
    }
    // Remember the walls
    oldWalls[ray][0] = proect_height;
    oldWalls[ray][1] = texture;
  }
}

void DrawUI()
{
  DrawFPS();
}

/* ________UTILS________ */
void ResetValues()
{
  memset(oldWalls, 0, sizeof(oldWalls));
  oldFPS = 0;
  fpsSize = 0;
  numRaysSize = 0;
  widthTextSize = 0;
  heightTextSize = 0;
}

void ResetDisplay()
{
  DrawBG();
  DrawNumRaysCount();
  DrawGameWidth();
  DrawGameHeight();
}

void UpdateCastSettings()
{
  DeltaAngle = Fow / NumRays;
  Dist = NumRays / tan(HalfFow);
  PrectCoeff = round(GameWidth / NumRays) * Dist * 10;
  Scale = round(GameWidth / NumRays);

  if (!isFullScreenGame)
  {
    ResetValues();
    ResetDisplay();
  }
  else
  {
    ResetValues();
    DrawBG();
  }
}

void Tick()
{
  SelectButton.tick();
  UpButton.tick();
  DownButton.tick();
}

void SetSettingsButton()
{
  SelectButton.setDebounce(ButtonsDebounce);
  UpButton.setDebounce(ButtonsDebounce);
  DownButton.setDebounce(ButtonsDebounce);
  SelectButton.setTimeout(ButtonsDebounce + 500);
}

/* ________PLAYER________ */
void PlayerMove(const float xAxis, const float yAxis)
{
  if (!((getWall(byte(x - playerSize) >> BitTile, byte(y) >> BitTile).isWall && xAxis > 0) ||
        (getWall(byte(x + playerSize) >> BitTile, byte(y) >> BitTile).isWall && xAxis < 0)))
    x -= xAxis;
  if (!((getWall(byte(x) >> BitTile, byte(y - playerSize) >> BitTile).isWall && yAxis > 0) ||
        (getWall(byte(x) >> BitTile, byte(y + playerSize) >> BitTile).isWall && yAxis < 0)))
    y -= yAxis;
}

/* ________INPUT_HANDLER________ */
unsigned long dFpsTime;

void MoveHandle()
{
  // Moving
  const int8_t joyX = analogRead(JoyX_Pin) * startJoyDeviderX - 1;
  const int8_t joyY = round(analogRead(JoyY_Pin) * startJoyDeviderY - 1);

  if (joyX || joyY)
  {
    const unsigned int millisec = millis();
    const byte dTimeMovement = millisec - dFpsTime;
    dFpsTime = millisec;

    // Update move speed with acceleration
    if (joyX)
    {
      const float step = joyX * moveSpeed * dTimeMovement;
      PlayerMove(step * cos(angle), step * sin(angle));
    }

    // Update rotate speed with acceleration
    if (joyY)
    {
      angle -= joyY * rotateSpeed * dTimeMovement;
    }
  }
}

void ChangeNumRaysHandle()
{
  if (UpButton.isPress() && NumRays < MaxNumRays)
  {
    NumRays <<= 1;
    UpdateCastSettings();
  }
  if (DownButton.isPress() && NumRays > MinNumRays)
  {
    NumRays >>= 1;
    UpdateCastSettings();
  }
}

void ChangeWidthHandle()
{
  if (UpButton.isPress() && GameWidth < MaxGameWidth)
  {
    GameWidth <<= 1;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
  if (DownButton.isPress() && GameWidth > MinGameWidth)
  {
    GameWidth >>= 1;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
}

void ChangeHeightHandle()
{
  if (UpButton.isPress() && GameHeight < MaxGameHeight)
  {
    GameHeight <<= 1;
    UpdateCastSettings();
  }
  if (DownButton.isPress() && GameHeight > MinGameHeight)
  {
    GameHeight >>= 1;
    UpdateCastSettings();
  }
}

void InputHandle()
{
  MoveHandle();

  if (!isFullScreenGame)
  {
    if (SelectButton.isPress())
    {
      if (++changeableGameSetting > 2)
        changeableGameSetting = 0;
      ResetValues();
      ResetDisplay();
    }

    switch (changeableGameSetting)
    {
    case 0:
      ChangeNumRaysHandle();
      break;
    case 1:
      ChangeWidthHandle();
      break;
    case 2:
      ChangeHeightHandle();
      break;
    }
  }
  if (SelectButton.isHolded())
  {
    if (isFullScreenGame)
    {
      GameWidth = MaxGameWidth;
      GameHeight = MaxGameHeight;
    }
    else
    {
      GameWidth = DisplayWidth;
      GameHeight = DisplayHeight;
    }
    NumRays = GameWidth;
    isFullScreenGame = !isFullScreenGame;
    UpdateCastSettings();
  }
}

/* ________RAY_CASTING________ */
void RaysCasting()
{
  byte castedWalls[NumRays][3];
  float cur_angle = angle - HalfFow;

  // For through the rays
  for (byte ray = 0; ray < NumRays; ray++)
  {
    const float sin_a = sin(cur_angle);
    const float cos_a = cos(cur_angle);
    const float cos_diff_angle = cos(angle - cur_angle);

    // verticals & horizontals
    const int8_t dx = (cos_a >= 0) ? 1 : -1;
    const int8_t dy = (sin_a >= 0) ? 1 : -1;
    const int8_t dxTile = dx << BitTile;
    const int8_t dyTile = dy << BitTile;
    byte px = ((byte(x) >> BitTile) << BitTile) + ((dx == 1) ? Tile : 0);
    byte py = ((byte(y) >> BitTile) << BitTile) + ((dy == 1) ? Tile : 0);

    float depth_v = 0;
    float depth_h = 0;
    float yv = 0;
    float xh = 0;

    bool isV_Collided = false;
    bool isH_Collided = false;
    byte wallV_Pos[2] = {0, 0};
    byte wallH_Pos[2] = {0, 0};
    Wall wallV, wallH;

    for (byte i = 0; i < MaxDepthTile && !((isV_Collided && isH_Collided) || (isV_Collided && (depth_v < depth_h))); i++)
    {
      if (!isV_Collided)
      {
        depth_v = (px - x) / cos_a;
        yv = y + depth_v * sin_a;
        wallV = getWall((px + dx) >> BitTile, byte(yv) >> BitTile);
        if (wallV.isWall)
        {
          isV_Collided = true;
          wallV_Pos[0] = ((px + dx) >> BitTile);
          wallV_Pos[1] = (byte(yv) >> BitTile);
        }
        if (wallV.isError || depth_v > MaxDepth || MapColumnsTile < yv || yv < 0 || px < 0 || px > MapRowsTile)
          isV_Collided = true;
        px += dxTile;
      }
      if (!isH_Collided)
      {
        depth_h = (py - y) / sin_a;
        xh = x + depth_h * cos_a;
        wallH = getWall(byte(xh) >> BitTile, (py + dy) >> BitTile);
        if (wallH.isWall)
        {
          isH_Collided = true;
          wallH_Pos[0] = (byte(xh) >> BitTile);
          wallH_Pos[1] = ((py + dy) >> BitTile);
        }
        if (wallH.isError || depth_h > MaxDepth || MapRowsTile < xh || xh < 0 || py > MapColumnsTile || py < 0)
          isH_Collided = true;
        py += dyTile;
      }
    }

    const float depth = (depth_v < depth_h) ? depth_v * cos_diff_angle : depth_h * cos_diff_angle;

    const uint16_t proect_height = PrectCoeff / depth;
    castedWalls[ray][0] = (proect_height < GameHeight) ? proect_height : GameHeight;
    // if (depth < MaxDepth - Tile)
    // {
    if (depth_v < depth_h)
    {
      castedWalls[ray][1] = wallV_Pos[0];
      castedWalls[ray][2] = wallV_Pos[1];
    }
    else
    {
      castedWalls[ray][1] = wallH_Pos[0];
      castedWalls[ray][2] = wallH_Pos[1];
    }
    // }
    // else
    // {
    //   castedWalls[ray][1] = 0;
    //   castedWalls[ray][2] = 0;
    // }

    // Updating the angle
    cur_angle += DeltaAngle;
  }

  DrawCastWalls(castedWalls);
}

void setup()
{
// Init
#ifdef DEBUG_ENABLE
  Serial.begin(9600);
#endif

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(45);
  tft.setSPISpeed(4294967295UL); // If you are having problems with the display, try removing this line

  startJoyDeviderX = 1.0 / analogRead(JoyX_Pin);
  startJoyDeviderY = 1.0 / analogRead(JoyY_Pin);

  MapInit();
  UpdateCastSettings();
  SetSettingsButton();

  pinMode(SelectButtonPin, INPUT_PULLUP);
}

void loop()
{
  // DEBUG(freeRam());
  fps_count++;

  // Update
  Tick();
  InputHandle();

  // Rendering
  RaysCasting();
  DrawUI();

  // FPS counter
  if (millis() - tick >= 1000)
  {
    tick += 1000;
    fps = fps_count;
    fps_count = 0;
  }
}