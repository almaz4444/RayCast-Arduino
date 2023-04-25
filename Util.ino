void UpdateCastSettings() {
  DeltaAngle = Fow / NumRays;
  Dist       = NumRays / tan(HalfFow);
  PrectCoeff = round(GameWidth / NumRays) * Dist * 10;
  Scale      = round(GameWidth / NumRays);

  ResetValues();
  ResetDisplay();
}

void ResetValues() {
  memset(oldWalls, 0, sizeof(oldWalls));
  oldFPS = 0;
  fpsSize = 0;
  numRaysSize = 0;
  widthTextSize = 0;
  heightTextSize = 0;
}

void ResetDisplay() {
  DrawBG();
  DrawNumRaysCount();
  DrawGameWidth();
  DrawGameHeight();
}

void Tick() {
  JoyButton.tick();
  UpButton.tick();
  DownButton.tick();
}

void SetSettingsButton() {
  JoyButton.setDebounce(ButtonsDebounce);
  UpButton.setDebounce(ButtonsDebounce);
  DownButton.setDebounce(ButtonsDebounce);
}