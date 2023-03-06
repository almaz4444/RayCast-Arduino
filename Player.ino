void movePlayer() {
  // Reading joysticks
  float paddleX = (analogRead(JoyY_Pin) - 1064 / 2) / 320;
  float paddleY = (analogRead(JoyX_Pin) - 1064 / 2) / 320;

  // Moving
  if(paddleY > 0) {
    x += -speed * cos(angle);
    y += -speed * sin(angle);
  } else if(paddleY < 0) {
    x += speed * cos(angle);
    y += speed * sin(angle);
  }
  angle -= paddleX / 10;
}