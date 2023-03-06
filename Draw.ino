#include <TFT.h>

byte oldFPS;
float oldPlayerX, oldPlayerY;

void DrawMiniMap() {
  // Map
  TFTscreen.fill(MiniMapColor[0], MiniMapColor[1], MiniMapColor[2]);
  for(int i = 0; i < wallsCount; i++) {
    TFTscreen.rect(MiniMapOffset[0] + Map[i][0] / Tile * TileInMiniMap, MiniMapOffset[1] + Map[i][1] / Tile * TileInMiniMap, TileInMiniMap, TileInMiniMap);
  }

  // Look line
  // TFTscreen.drawLine(MiniMapOffset[0] + x / Tile * TileInMiniMap,
  //                    MiniMapOffset[1] + y / Tile * TileInMiniMap,
  //                    MiniMapOffset[0] + x / Tile * TileInMiniMap + 5 * cos(angle),
  //                    MiniMapOffset[1] + y / Tile * TileInMiniMap + 5 * sin(angle),
  //                    TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  
  // // Player
  // TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
  // TFTscreen.circle(MiniMapOffset[0] + x / Tile * TileInMiniMap, MiniMapOffset[1] + y / Tile * TileInMiniMap, playerSize);
  
  // Player
  if(x != oldPlayerX || y != oldPlayerY) {
    TFTscreen.fill(0, 0, 0);
    TFTscreen.circle(MiniMapOffset[0] + oldPlayerX / Tile * TileInMiniMap, MiniMapOffset[1] + oldPlayerY / Tile * TileInMiniMap, playerSize);
    TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
    TFTscreen.circle(MiniMapOffset[0] + x / Tile * TileInMiniMap, MiniMapOffset[1] + y / Tile * TileInMiniMap, playerSize);
  } 
  oldPlayerX = x;
  oldPlayerY = y;
}

void DrawMap() {
  // Map
  TFTscreen.fill(MiniMapColor[0], MiniMapColor[1], MiniMapColor[2]);
  for(int i = 0; i < wallsCount; i++) {
    TFTscreen.rect(Map[i][0], Map[i][1], Tile, Tile);
  }
  
  // Player
  if(x != oldPlayerX || y != oldPlayerY) {
    TFTscreen.fill(0, 0, 0);
    TFTscreen.circle(oldPlayerX, oldPlayerY, playerSize);
    TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
    TFTscreen.circle(x, y, playerSize);
  } 
  oldPlayerX = x;
  oldPlayerY = y;
}

void DrawBG(bool is3D) {
  if(is3D) {
    TFTscreen.fill(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
    TFTscreen.rect(0, 0, Width, Height / 2);
    TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
    TFTscreen.rect(0, Height / 2, Width, Height / 2);
  } else {
    TFTscreen.background(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
  }
}

void DrawFPS(bool isLoopDraw, bool is3D) {
  if(oldFPS != fps || isLoopDraw) {
    TFTscreen.textSize(FPS_TextSize);
    if(is3D) TFTscreen.stroke(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
    else TFTscreen.stroke(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
    TFTscreen.text(("FPS:" + (String)oldFPS).c_str(), 5, 5);
    if(is3D) TFTscreen.stroke(FPSColor2D[0], FPSColor2D[1], FPSColor2D[2]);
    else TFTscreen.stroke(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
    TFTscreen.text(("FPS:" + (String)fps).c_str(), 5, 5);
    TFTscreen.noStroke();
  }
  oldFPS = fps;
}