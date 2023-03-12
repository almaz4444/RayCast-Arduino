byte oldFPS;
float oldPlayerX, oldPlayerY, oldAngle;

void DrawMap(bool isClearDisplay) {
  // Draw map if not this on display
  if(isClearDisplay) {
    for(byte i = 0; i < wallsCount; i++) {
      if(Map[i].color == 0) TFTscreen.fill(255, 0, 0);
      else if(Map[i].color == 1) TFTscreen.fill(0, 255, 0);
      else if(Map[i].color == 2) TFTscreen.fill(0, 0, 255);
      TFTscreen.rect((Map[i].x >> BitTile) * TileInMiniMap, (Map[i].y >> BitTile) * TileInMiniMap, Width / MapRows, TileInMiniMap);
    }
  }
  
  // Player
  if(x != oldPlayerX || y != oldPlayerY || angle != oldAngle || isClearDisplay) {
    TFTscreen.fill(0, 0, 0);
    TFTscreen.circle((oldPlayerX / Tile) * TileInMiniMap, (oldPlayerY / Tile) * TileInMiniMap, (playerSize / (float)Tile) * TileInMiniMap);
    TFTscreen.drawLine((oldPlayerX / Tile) * TileInMiniMap,
                       (oldPlayerY / Tile) * TileInMiniMap,
                       (oldPlayerX / Tile) * TileInMiniMap + 4 * cos(oldAngle),
                       (oldPlayerY / Tile) * TileInMiniMap + 4 * sin(oldAngle),
                       TFTscreen.Color565(0, 0, 0));
    TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
    TFTscreen.circle((x / Tile) * TileInMiniMap, (y / Tile) * TileInMiniMap, (playerSize / (float)Tile) * TileInMiniMap);
    TFTscreen.drawLine((x / Tile) * TileInMiniMap,
                       (y / Tile) * TileInMiniMap,
                       (x / Tile) * TileInMiniMap + 4 * cos(angle),
                       (y / Tile) * TileInMiniMap + 4 * sin(angle),
                       TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  }
  oldPlayerX = x;
  oldPlayerY = y;
  oldAngle = angle;
}

void DrawBG() {
  if(Is3D) {
    TFTscreen.fill(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
    TFTscreen.rect(0, 0, Width, Height >> 1);
    TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
    TFTscreen.rect(0, Height >> 1, Width, Height >> 1);
  } else {
    TFTscreen.background(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
  }
}

void DrawFPS(boolean isLoopDraw) {
  if(isLoopDraw || oldFPS != fps) {
    TFTscreen.textSize(FPS_TextSize);
    
    if(Is3D) TFTscreen.stroke(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
    else TFTscreen.stroke(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
    TFTscreen.text(("FPS:" + (String)oldFPS).c_str(), 5, 5);
    
    if(Is3D) TFTscreen.stroke(FPSColor[0], FPSColor[1], FPSColor[2]);
    else TFTscreen.stroke(FPSColor[0], FPSColor[1], FPSColor[2]);
    TFTscreen.text(("FPS:" + (String)fps).c_str(), 5, 5);
    
    TFTscreen.noStroke();
  }
  oldFPS = fps;
}