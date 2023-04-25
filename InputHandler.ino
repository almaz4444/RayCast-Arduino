unsigned long dFpsTime;

void InputHandle() {
  MoveHandle();

  if(JoyButton.isPress()) {
    if(++changeableGameSetting > 2) changeableGameSetting = 0;
    ResetValues();
    ResetDisplay();
  }

  switch(changeableGameSetting) {
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

void MoveHandle() {
  // Moving
  const int8_t joyX = analogRead(JoyX_Pin) * startJoyDeviderX - 1;
  const int8_t joyY = round(analogRead(JoyY_Pin) * startJoyDeviderY - 1);

  if (joyX || joyY) {
    const unsigned int millisec = millis();
    const byte dTimeMovement = millisec - dFpsTime;
    dFpsTime = millisec;

    // Update move speed with acceleration
    if (joyX) {
      const float step = joyX * moveSpeed * dTimeMovement;
      PlayerMove(step * cos(angle), step * sin(angle));
    }

    // Update rotate speed with acceleration
    if (joyY) {
      angle -= joyY * rotateSpeed * dTimeMovement;
    }
  }
}

void ChangeNumRaysHandle() {
  if (UpButton.isPress() && NumRays < MaxNumRays) {
    NumRays <<= 1;
    UpdateCastSettings();
  }
  if (DownButton.isPress() && NumRays > MinNumRays) {
    NumRays >>= 1;
    UpdateCastSettings();
  }
}

void ChangeWidthHandle() {
  if (UpButton.isPress() && GameWidth < MaxGameWidth) {
    GameWidth <<= 1;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
  if (DownButton.isPress() && GameWidth > MinGameWidth) {
    GameWidth >>= 1;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
}

void ChangeHeightHandle() {
  if (UpButton.isPress() && GameHeight < MaxGameHeight) {
    GameHeight <<= 1;
    UpdateCastSettings();
  }
  if (DownButton.isPress() && GameHeight > MinGameHeight) {
    GameHeight >>= 1;
    UpdateCastSettings();
  }
}