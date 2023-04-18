byte oldFPS;

void DrawBG() {
  tft.fillRect(0, 0, Width, HeightField, SkyColor);
  tft.fillRect(0, HeightField, Width, HeightField, FlorColor);
}

void DrawFPS() {
  // tft.setTextSize(FPS_TextSize);
  tft.setCursor(5, 5);
  
  if(oldFPS != fps) {
    tft.setTextColor(SkyColor);
    tft.print((String)oldFPS);
    tft.setCursor(5, 5);

    oldFPS = fps;
  }
    
  tft.setTextColor(FPSColor);
  tft.print((String)fps);
}