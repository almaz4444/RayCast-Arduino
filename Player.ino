void movePlayer() {
  // Reading joysticks
  float paddleX = (analogRead(JoyY_Pin) - 1064 / 2) / 320;
  float paddleY = (analogRead(JoyX_Pin) - 1064 / 2) / 320;

  // Moving
  if(paddleY > 0) {
    x += -moveSpeed * (millis() - dFpsTime) / 200 * cos(angle);
    y += -moveSpeed * (millis() - dFpsTime) / 200 * sin(angle);
  } else if(paddleY < 0) {
    x += moveSpeed * (millis() - dFpsTime)  / 200 * cos(angle);
    y += moveSpeed * (millis() - dFpsTime)  / 200 * sin(angle);
  }
  angle -= paddleX * rotateSpeed * (millis() - dFpsTime);
}