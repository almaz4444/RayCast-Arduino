void UpdateCastSettings() {
  DeltaAngle = Fow / NumRays;
  Dist       = NumRays / (10 * tan(HalfFow));
  PrectCoeff = round(GameWidth / NumRays) * Dist * 10;
  Scale      = round(GameWidth / NumRays);

  ResetValues();
  ResetDisplay();
}

void ResetValues() {
  memset(oldWalls, 0, sizeof(oldWalls));
  oldFPS = 0;
  fpsSize = 0;
  widthTextSize = 0;
  heightTextSize = 0;
  numRaysSize = 0;
}

void ResetDisplay() {
  DrawBG();
  DrawNumRaysCount();
  DrawGameWidth();
  DrawGameHeight();
}