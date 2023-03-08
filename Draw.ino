#include <TFT.h>

byte oldFPS;
float oldPlayerX, oldPlayerY, oldAngle;

// void DrawMiniMap() {
//   // Map
//   TFTscreen.fill(MiniMapColor[0], MiniMapColor[1], MiniMapColor[2]);
//   for(int i = 0; i < wallsCount; i++) {
//     TFTscreen.rect(MiniMapOffset[0] + Map[i][0] / Tile * TileInMiniMap, MiniMapOffset[1] + Map[i][1] / Tile * TileInMiniMap, TileInMiniMap, TileInMiniMap);
//   }

//   // Look line
//   TFTscreen.drawLine(MiniMapOffset[0] + x / Tile * TileInMiniMap,
//                      MiniMapOffset[1] + y / Tile * TileInMiniMap,
//                      MiniMapOffset[0] + x / Tile * TileInMiniMap + 5 * cos(angle),
//                      MiniMapOffset[1] + y / Tile * TileInMiniMap + 5 * sin(angle),
//                      TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  
//   // Player
//   TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
//   TFTscreen.circle(MiniMapOffset[0] + x / Tile * TileInMiniMap, MiniMapOffset[1] + y / Tile * TileInMiniMap, playerSize);
// }

void DrawMap() {
  // Map
  for(int i = 0; i < wallsCount; i++) {
    if(Map[i].color == 0) TFTscreen.fill(255, 0, 0);
    else if(Map[i].color == 1) TFTscreen.fill(0, 255, 0);
    else if(Map[i].color == 2) TFTscreen.fill(0, 0, 255);
    TFTscreen.rect(Map[i].x / Tile * (Width / MapRows), Map[i].y / Tile * (Height / StringMap[0].length()), Width / MapRows, Height / StringMap[0].length());
  }
  
  // Player
  if(x != oldPlayerX || y != oldPlayerY || angle != oldAngle) {
    TFTscreen.fill(0, 0, 0);
    TFTscreen.circle(oldPlayerX / Tile * (Width / MapRows), oldPlayerY / Tile * (Width / MapRows), playerSize);
    TFTscreen.drawLine(oldPlayerX / Tile * (Width / MapRows),
                   oldPlayerY / Tile * (Width / MapRows),
                   oldPlayerX / Tile * (Width / MapRows) + 3 * cos(oldAngle),
                   oldPlayerY / Tile * (Width / MapRows) + 3 * sin(oldAngle),
                   TFTscreen.Color565(0, 0, 0));
    TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
    TFTscreen.circle(x / Tile * (Width / MapRows), y / Tile * (Width / MapRows), playerSize);
    TFTscreen.drawLine(x / Tile * (Width / MapRows),
                       y / Tile * (Width / MapRows),
                       x / Tile * (Width / MapRows) + 3 * cos(angle),
                       y / Tile * (Width / MapRows) + 3 * sin(angle),
                       TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  }
  oldPlayerX = x;
  oldPlayerY = y;
  oldAngle = angle;
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

void DrawFPS(bool isLoopDraw) {
  if(oldFPS != fps || isLoopDraw) {
    TFTscreen.textSize(FPS_TextSize);
    
    if(Is3D) TFTscreen.stroke(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
    else TFTscreen.stroke(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
    TFTscreen.text(("FPS:" + (String)oldFPS).c_str(), 5, 5);
    
    if(Is3D) TFTscreen.stroke(FPSColor3D[0], FPSColor3D[1], FPSColor3D[2]);
    else TFTscreen.stroke(FPSColor2D[0], FPSColor2D[1], FPSColor2D[2]);
    TFTscreen.text(("FPS:" + (String)fps).c_str(), 5, 5);
    
    TFTscreen.noStroke();
  }
  oldFPS = fps;
}