byte dFpsTime;

void movePlayer() {
  // Moving
  const byte millisec = millis();
  const byte dTimeMovement = millisec - dFpsTime;
  dFpsTime = millisec;
  
  const float step = int8_t(analogRead(JoyX_Pin) * startJoyX - 1) * moveSpeed * dTimeMovement;
  
  if(step) PlayerMove(step * cos(angle), step * sin(angle));
  angle -= round(analogRead(JoyY_Pin) * startJoyY - 1) * rotateSpeed * dTimeMovement;
}

void PlayerMove(const float xAxis, const float yAxis) {
  if(!((getWall(byte(x - playerSize) >> BitTile, byte(y) >> BitTile).isWall && xAxis > 0) ||
     (getWall(byte(x + playerSize) >> BitTile, byte(y) >> BitTile).isWall && xAxis < 0))) x -= xAxis;
  if(!((getWall(byte(x) >> BitTile, byte(y - playerSize) >> BitTile).isWall && yAxis > 0) ||
     (getWall(byte(x) >> BitTile, byte(y + playerSize) >> BitTile).isWall && yAxis < 0))) y -= yAxis;
}