void DrawBG() {
  tft.fillScreen(BlackColor);
  tft.fillRect(0, 0, GameWidth, GameHeightField, SkyColor);
  tft.fillRect(0, GameHeightField, GameWidth, GameHeightField, FlorColor);
}

void DrawUI() {
  DrawFPS();
}

uint16_t oldFPS;
size_t fpsSize;
void DrawFPS() {
  if(!fpsSize) {
    tft.setCursor(FPS_TextPos[0], FPS_TextPos[1]);
    tft.setTextSize(UI_TextSize);
    tft.setTextColor(NormalColor);
    fpsSize = tft.println("FPS:") << 1;
  }
  if(oldFPS != fps) {
    tft.setTextSize(UI_TextSize);
    tft.setCursor(FPS_TextPos[0], FPS_TextPos[1] + fpsSize);

    tft.setTextColor(BlackColor);
    tft.print((String)oldFPS);
    tft.setCursor(FPS_TextPos[0], FPS_TextPos[1] + fpsSize);
    
    tft.setTextColor(NormalColor);
    tft.print((String)fps);

    oldFPS = fps;
  }
}

size_t numRaysSize;
void DrawNumRaysCount() {
  tft.setTextSize(UI_TextSize);
  (changeHandle == 0)? tft.setTextColor(SelectedColor) : tft.setTextColor(NormalColor);
  tft.setCursor(NumRays_TextPos[0], NumRays_TextPos[1] + numRaysSize);
  if(!numRaysSize) {
    numRaysSize = tft.println("RC:") << 1;
    tft.setCursor(NumRays_TextPos[0], NumRays_TextPos[1] + numRaysSize);
  }
  tft.print((String)NumRays);
}

size_t widthTextSize;
void DrawGameWidth() {
  tft.setTextSize(UI_TextSize);
  (changeHandle == 1)? tft.setTextColor(SelectedColor) : tft.setTextColor(NormalColor);
  tft.setCursor(Width_TextPos[0], Width_TextPos[1] + widthTextSize);
  if(!widthTextSize) {
    widthTextSize = tft.println("W:") << 1;
    tft.setCursor(Width_TextPos[0], Width_TextPos[1] + widthTextSize);
  }
  tft.print((String)GameWidth);
}

size_t heightTextSize;
void DrawGameHeight() {
  tft.setTextSize(UI_TextSize);
  (changeHandle == 2)? tft.setTextColor(SelectedColor) : tft.setTextColor(NormalColor);
  tft.setCursor(Height_TextPos[0], Height_TextPos[1] + heightTextSize);
  if(!heightTextSize) {
    heightTextSize = tft.println("H:") << 1;
    tft.setCursor(Height_TextPos[0], Height_TextPos[1] + heightTextSize);
  }
  tft.print((String)GameHeight);
}

void DrawCastWalls(uint16_t castedWalls[Width][2]) {
  for (byte ray = 0; ray < NumRays; ray++) {
    const byte proect_height = castedWalls[ray][0];
    const uint16_t texture = castedWalls[ray][1];
    const byte oldWallProjectHeight = oldWalls[ray][0];
    const byte oldWallProjectHeightField = (oldWallProjectHeight >> 1);
    const byte proectHeightField = (proect_height >> 1);
    const byte brickY = GameHeightField - proectHeightField;
    const byte differenceWallsHeight = oldWallProjectHeightField - proectHeightField;
    const byte brickX = ray * Scale;

    // fill the voids
    if (oldWallProjectHeight > proect_height) {
      const byte oldBrickYTop = GameHeightField - oldWallProjectHeightField;
      tft.fillRect(brickX, oldBrickYTop, Scale, differenceWallsHeight, SkyColor);
      tft.fillRect(brickX, oldBrickYTop + oldWallProjectHeightField + proectHeightField, Scale, differenceWallsHeight + 1, FlorColor);
    }
    
    // Render of new walls
    if(oldWalls[ray][1] != texture) {
      tft.fillRect(brickX, brickY, Scale, proect_height, texture);
    } else if(oldWallProjectHeight < proect_height) {
      const byte oldBrickY = proectHeightField - oldWallProjectHeightField;
      tft.fillRect(brickX, brickY, Scale, oldBrickY, texture);
      tft.fillRect(brickX, GameHeightField + oldWallProjectHeightField, Scale, oldBrickY, texture);
    }
    // Remember the walls
    oldWalls[ray][0] = proect_height;
    oldWalls[ray][1] = texture;
  }
}