void movePlayer() {
  // Reading joysticks
  int paddleX = (analogRead(JoyY_Pin) - 532) >> 5;
  int8_t paddleY = (analogRead(JoyX_Pin) - 520) * 0.002;

  if(paddleY > 0) paddleX = -paddleX;

  // Moving       430, 368
  byte dTimeMovement = millis() - dFpsTime;
    
  float step = moveSpeed * dTimeMovement;
  int8_t cos_step = step * cos(angle);
  int8_t sin_step = step * sin(angle);
  x -= paddleY * cos_step * 0.01;
  y -= paddleY * sin_step * 0.01;
  angle -= paddleX * rotateSpeed / 10.0 * dTimeMovement;

  collision(paddleY * cos_step, paddleY * sin_step);
}

void collision(int8_t cos_step, int8_t sin_step) {
  if(isInMap(byte(x - playerSize) >> BitTile, byte(y) >> BitTile) != -1 && cos_step > 0) x += cos_step * 0.01;
  else if(isInMap(byte(x + playerSize) >> BitTile, byte(y) >> BitTile) != -1 && cos_step < 0) x += cos_step * 0.01;
  if(isInMap(byte(x) >> BitTile, byte(y - playerSize) >> BitTile) != -1 && sin_step > 0) y += sin_step * 0.01;
  else if(isInMap(byte(x) >> BitTile, byte(y + playerSize) >> BitTile) != -1 && sin_step < 0) y += sin_step * 0.01;
}