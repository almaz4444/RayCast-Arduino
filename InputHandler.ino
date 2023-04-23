unsigned long dFpsTime;

void InputHandle() {
  MoveHandle();

  if(JoyButton.click()) {
    if(changeHandle < 2) changeHandle++;
    else changeHandle = 0;
    ResetValues();
    ResetDisplay();
  }

  Serial.println(changeHandle);

  switch(changeHandle) {
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

float move_acceleration = 0.0001;
float rotate_acceleration = 0.0001;
float current_move_speed = 0;
float current_rotate_speed = 0;

void MoveHandle() {
  // Moving
  const int joyX = analogRead(JoyX_Pin) * startJoyX - 1;
  const int joyY = round(analogRead(JoyY_Pin) * startJoyY - 1);

  if (joyX || joyY) {
    unsigned long millisec = millis();
    const byte dTimeMovement = millisec - dFpsTime;
    dFpsTime = millisec;

    // Update move speed with acceleration
    if (joyX) {
      current_move_speed += move_acceleration * dTimeMovement;
      current_move_speed = min(current_move_speed, moveSpeed);
      const float step = joyX * current_move_speed * dTimeMovement;
      PlayerMove(step * cos(angle), step * sin(angle));
    } else {
      current_move_speed = max(0, current_move_speed - move_acceleration * dTimeMovement);
    }

    // Update rotate speed with acceleration
    if (joyY) {
      current_rotate_speed += rotate_acceleration * dTimeMovement;
      current_rotate_speed = min(current_rotate_speed, rotateSpeed);
      angle -= joyY * current_rotate_speed * dTimeMovement;
    } else {
      current_rotate_speed = max(0, current_rotate_speed - rotate_acceleration * dTimeMovement);
    }
  }
}

void ChangeNumRaysHandle() {
  if(UpButton.click() && NumRays < GameWidth) {
    NumRays = NumRays << 1;
    UpdateCastSettings();
  }
  if(DownButton.click() && NumRays > 1) {
    NumRays = NumRays >> 1;
    UpdateCastSettings();
  }
}

void ChangeWidthHandle() {
  if(UpButton.click() && GameWidth < MaxGameWidth) {
    GameWidth += 10;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
  if(DownButton.click() && NumRays > 10) {
    GameWidth -= 10;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
}

void ChangeHeightHandle() {
  if(UpButton.click() && GameHeight < Height) {
    GameHeight += 10;
    UpdateCastSettings();
  }
  if(DownButton.click() && NumRays > 10) {
    GameHeight -= 10;
    UpdateCastSettings();
  }
}