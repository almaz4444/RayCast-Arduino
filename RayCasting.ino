uint oldWalls[NumRays][2];

void RaysCasting(const bool isClearDisplay) {
  // Init
  uint castedWalls[NumRays][2];
  float cur_angle = angle - HalfFow;
  const float angleIncrement = DeltaAngle; // Cache angle increment

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
    uint texture_v;
    uint texture_h;
    
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

    float depth; 
    uint color;
    
    if (depth_v < depth_h) {
      depth = depth_v * cos_diff_angle;
      color = texture_v;
    } else {
      depth = depth_h * cos_diff_angle;
      color = texture_h;
    }

    castedWalls[ray][0] = min(Height, PrectCoeff / max(0, depth));
    castedWalls[ray][1] = (depth >= MaxDepth - Tile)? BlackColor : color;

    // Updating the angle
    cur_angle += angleIncrement;
  }

  for (byte ray = 0; ray < NumRays; ray++) {
    const byte proect_height = castedWalls[ray][0];
    const uint texture = castedWalls[ray][1];
    const byte oldWallProjectHeight = oldWalls[ray][0];
    const byte oldWallProjectHeightField = (oldWallProjectHeight >> 1);
    const byte proectHeightField = (proect_height >> 1);
    const byte brickY = HeightField - proectHeightField;
    const byte differenceWallsHeight = oldWallProjectHeightField - proectHeightField;
    const bool isWallChanged = oldWalls[ray][1] != texture || isClearDisplay;
    const byte brickX = ray * Scale;

    // fill the voids
    if (oldWallProjectHeight > proect_height) {
      const byte oldBrickYTop = HeightField - oldWallProjectHeightField;
      tft.fillRect(brickX, oldBrickYTop, Scale, differenceWallsHeight, SkyColor);
      tft.fillRect(brickX, oldBrickYTop + oldWallProjectHeightField + proectHeightField, Scale, differenceWallsHeight + 1, FlorColor);
    }
    
    // Render of new walls
    if(isWallChanged) {
      tft.fillRect(brickX, brickY, Scale, proect_height, texture);
    } else if(oldWallProjectHeight < proect_height) {
      const byte oldBrickY = proectHeightField - oldWallProjectHeightField;
      tft.fillRect(brickX, brickY, Scale, oldBrickY, texture);
      tft.fillRect(brickX, HeightField + oldWallProjectHeightField, Scale, oldBrickY, texture);
    }
    // Remember the walls
    oldWalls[ray][0] = proect_height;
    oldWalls[ray][1] = texture;
  }
}