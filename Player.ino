void PlayerMove(const float xAxis, const float yAxis) {
  if(!((getWall(byte(x - playerSize) >> BitTile, byte(y) >> BitTile).isWall && xAxis > 0) ||
     (getWall(byte(x + playerSize) >> BitTile, byte(y) >> BitTile).isWall && xAxis < 0))) x -= xAxis;
  if(!((getWall(byte(x) >> BitTile, byte(y - playerSize) >> BitTile).isWall && yAxis > 0) ||
     (getWall(byte(x) >> BitTile, byte(y + playerSize) >> BitTile).isWall && yAxis < 0))) y -= yAxis;
}