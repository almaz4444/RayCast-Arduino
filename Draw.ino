byte oldFPS;
float oldPlayerX, oldPlayerY, oldAngle;

void DrawMap(bool isClearDisplay) {
  // Draw map if not this on display
  if(isClearDisplay) {
    for(byte i = 0; i < wallsCount; i++) {
      if(Map[i].color == 0) TFTscreen.fill(255, 0, 0);
      else if(Map[i].color == 1) TFTscreen.fill(0, 255, 0);
      else if(Map[i].color == 2) TFTscreen.fill(0, 0, 255);
      TFTscreen.rect(Map[i].x / Tile * (Width / MapRows), Map[i].y / Tile * (Height / StringMap[0].length()), Width / MapRows, Height / StringMap[0].length());
    }
  }
  
  // Player
  if(x != oldPlayerX || y != oldPlayerY || angle != oldAngle || isClearDisplay) {
    TFTscreen.fill(0, 0, 0);
    TFTscreen.circle(oldPlayerX / Tile * (Width / MapRows), oldPlayerY / Tile * (Width / MapRows), playerSize);
    TFTscreen.drawLine(oldPlayerX / Tile * (Width / MapRows),
                       oldPlayerY / Tile * (Width / MapRows),
                       oldPlayerX / Tile * (Width / MapRows) + 4 * cos(oldAngle),
                       oldPlayerY / Tile * (Width / MapRows) + 4 * sin(oldAngle),
                       TFTscreen.Color565(0, 0, 0));
    TFTscreen.fill(PlayerColor[0], PlayerColor[1], PlayerColor[2]);
    TFTscreen.circle(x / Tile * (Width / MapRows), y / Tile * (Width / MapRows), playerSize);
    TFTscreen.drawLine(x / Tile * (Width / MapRows),
                       y / Tile * (Width / MapRows),
                       x / Tile * (Width / MapRows) + 4 * cos(angle),
                       y / Tile * (Width / MapRows) + 4 * sin(angle),
                       TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  }
  oldPlayerX = x;
  oldPlayerY = y;
  oldAngle = angle;
}

void DrawBG() {
  if(Is3D) {
    TFTscreen.fill(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
    TFTscreen.rect(0, 0, Width, Height / 2);
    TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
    TFTscreen.rect(0, Height / 2, Width, Height / 2);
  } else {
    TFTscreen.background(BGColor2D[0], BGColor2D[1], BGColor2D[2]);
  }
}

void DrawFPS(boolean isLoopDraw) {
  if(oldFPS != fps || isLoopDraw) {
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