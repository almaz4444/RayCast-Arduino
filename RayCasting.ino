
void RaysCasting() {
  // Init
  float cur_angle = angle - HalfFow;
  
  uint16_t castedWalls[NumRays][2];

  // For through the rays
  for (byte ray = 0; ray < NumRays; ray++) {
    const float sin_a = sin(cur_angle);
    const float cos_a = cos(cur_angle);
    const float cos_diff_angle = cos(angle - cur_angle);

    // verticals & horizontals
    const int8_t dx = (cos_a >= 0) ? 1 : -1;
    const int8_t dy = (sin_a >= 0) ? 1 : -1;
    const int8_t dxTile = dx << BitTile;
    const int8_t dyTile = dy << BitTile;
    byte px = ((byte(x) >> BitTile) << BitTile) + ((dx == 1) ? Tile : 0);
    byte py = ((byte(y) >> BitTile) << BitTile) + ((dy == 1) ? Tile : 0);
    
    float depth_v, depth_h;
    float yv, xh;
    uint16_t texture_v;
    uint16_t texture_h;
    
    boolean isV_Collided = false;
    boolean isH_Collided = false;
    Wall wallV, wallH;
    
    for (byte i = 0; i < MaxDepthTile && !((isV_Collided && isH_Collided) || (isV_Collided && (depth_v < depth_h))); i++) {
      if (!isV_Collided) {
        depth_v = (px - x) / cos_a;
        yv = y + depth_v * sin_a;
        wallV = getWall((px + dx) >> BitTile, byte(yv) >> BitTile);
        if(wallV.isWall) {
          texture_v = wallV.color;
          isV_Collided = true;
        } else if(wallV.isError) isV_Collided = true;
        if (depth_v > MaxDepth || MapColumnsTile < yv || yv < 0 || px < 0 || px > MapRowsTile) isV_Collided = true;
        px += dxTile;
      }
      if (!isH_Collided) {
        depth_h = (py - y) / sin_a;
        xh = x + depth_h * cos_a;
        wallH = getWall(byte(xh) >> BitTile, (py + dy) >> BitTile);
        if(wallH.isWall) {
          texture_h = wallH.color;
          isH_Collided = true;
        } else if(wallH.isError) isH_Collided = true;
        if (depth_h > MaxDepth || MapRowsTile < xh || xh < 0 || py > MapColumnsTile || py < 0) isH_Collided = true;
        py += dyTile;
      }
    }

    const float depth = (depth_v < depth_h)? depth_v * cos_diff_angle : depth_h * cos_diff_angle;

    castedWalls[ray][0] = min(GameHeight, PrectCoeff / depth);
    castedWalls[ray][1] = (depth >= MaxDepth - Tile)? BlackColor : ((depth_v < depth_h)? texture_v : texture_h);

    // Updating the angle
    cur_angle += DeltaAngle;
  }

  DrawCastWalls(castedWalls);
}