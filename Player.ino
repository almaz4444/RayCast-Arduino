#define deathZoneX 6
#define deathZoneY 4

void movePlayer() {
  // Reading joysticks
  const int8_t paddleX = ((1.0 / startJoyX) * analogRead(JoyX_Pin) - 1) * 10;
  const int8_t paddleY = (paddleY > 0)? -((1.0 / startJoyY) * analogRead(JoyY_Pin) - 1) * 10 : ((1.0 / startJoyY) * analogRead(JoyY_Pin) - 1) * 10;

  // Moving
  const byte millisec = millis();
  const byte dTimeMovement = millisec - dFpsTime;
  dFpsTime = millisec;
  
  if(abs(paddleX) > deathZoneX) {
    const float  step     = moveSpeed * dTimeMovement;
    const float  xMove    = paddleX * step * cos(angle) * 0.01;
    const float  yMove    = paddleX * step * sin(angle) * 0.01;
    x -= xMove;
    y -= yMove;
    collision(xMove, yMove);
  }
  if(abs(paddleY) > deathZoneY) {
    angle -= paddleY * rotateSpeed * dTimeMovement;
  }
}

void collision(const float xMove, const float yMove) {
  if((getWall(byte(x - playerSize) >> BitTile, byte(y) >> BitTile).isWall && xMove > 0) ||
     (getWall(byte(x + playerSize) >> BitTile, byte(y) >> BitTile).isWall && xMove < 0)) x += xMove;
  if((getWall(byte(x) >> BitTile, byte(y - playerSize) >> BitTile).isWall && yMove > 0) ||
     (getWall(byte(x) >> BitTile, byte(y + playerSize) >> BitTile).isWall && yMove < 0)) y += yMove;
}