unsigned long dFpsTime;

void InputHandle() {
  MoveHandle();

  if(JoyButton.isPress()) {
    if(++changeHandle > 2) changeHandle = 0;
    ResetValues();
    ResetDisplay();
  }

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

const float move_acceleration = 0.000001;
const float rotate_acceleration = 0.000001;
float current_move_speed = 0;
float current_rotate_speed = 0;

void MoveHandle() {
  // Moving
  const int joyX = analogRead(JoyX_Pin) * startJoyX - 1;
  const int joyY = round(analogRead(JoyY_Pin) * startJoyY - 1);

  if (joyX || joyY) {
    const unsigned long millisec = millis();
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
  if(UpButton.isPress() && NumRays < GameWidth) {
    NumRays = NumRays << 1;
    UpdateCastSettings();
  }
  if(DownButton.isPress() && NumRays > 1) {
    NumRays = NumRays >> 1;
    UpdateCastSettings();
  }
}

void ChangeWidthHandle() {
  if(UpButton.isPress() && GameWidth < MaxGameWidth) {
    GameWidth = GameWidth << 1;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
  if(DownButton.isPress() && GameWidth > 4) {
    GameWidth = GameWidth >> 1;
    NumRays = GameWidth;
    UpdateCastSettings();
  }
}

void ChangeHeightHandle() {
  if(UpButton.isPress() && GameHeight < Height) {
    GameHeight = GameHeight << 1;
    UpdateCastSettings();
  }
  if(DownButton.isPress() && GameHeight > 4) {
    GameHeight = GameHeight >> 1;
    UpdateCastSettings();
  }
}