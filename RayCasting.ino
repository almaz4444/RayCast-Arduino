struct {
  byte proect_height;
  col_vec3 texture;
} oldWalls[NumRays];

struct {
  float depth = 0;
  col_vec3 texture;
  bool isCollided = false;
} castInfo;

void RayCast(const float sin_a, const float cos_a, const float x, const float y, const byte MaxDepthTile) {
  // verticals & horizontals glass
  int8_t dx = (cos_a >= 0) ? 1 : -1;
  int8_t dy = (sin_a >= 0) ? 1 : -1;
  int8_t dxTile = dx << BitTile;
  int8_t dyTile = dy << BitTile;
  byte px = ((byte(x) >> BitTile) << BitTile) + ((dx == 1) ? Tile : 0);
  byte py = ((byte(y) >> BitTile) << BitTile) + ((dy == 1) ? Tile : 0);

  float depth_v, depth_h;
  float depthGlass_h = 0;
  float depthGlass_v = 0;
  float yv, xh;
  col_vec3 texture_v;
  col_vec3 texture_h;

  boolean isV_Collided = false;
  boolean isH_Collided = false;
  for (byte i = 0; i < MaxDepthTile; i++) {
    if (!isV_Collided) {
      depth_v = (px - x) / cos_a;
      yv = y + depth_v * sin_a;
      Wall wallV = getWall((px + dx) >> BitTile, byte(yv) >> BitTile);
      if(wallV.isWall) {
        if (wallV.isGlass) {
          RayCast(-sin_a, cos_a, px, yv, MaxDepthTile - (i + 1));
          depthGlass_v = castInfo.depth;
          texture_v = castInfo.texture;
          isV_Collided = castInfo.isCollided;
        } else {
          texture_v = wallV.color;
          isV_Collided = true;
        }
      }
      if (depth_v > MaxDepth || MapColumnsTile < yv || yv < 0 || px < 0 || px > MapRowsTile) isV_Collided = true;
      px += dxTile;
    }
    if (!isH_Collided) {
      depth_h = (py - y) / sin_a;
      xh = x + depth_h * cos_a;
      Wall wallH = getWall(byte(xh) >> BitTile, (py + dy) >> BitTile);
      if(wallH.isWall) {
        if (wallH.isGlass) {
          RayCast(-sin_a, cos_a, xh, py, MaxDepthTile - (i + 1));
          depthGlass_h = castInfo.depth;
          texture_h = castInfo.texture;
          isH_Collided = castInfo.isCollided;
        } else {
          texture_h = wallH.color;
          isH_Collided = true;
        }
      }
      if (depth_h > MaxDepth || MapRowsTile < xh || xh < 0 || py > MapColumnsTile || py < 0) isH_Collided = true;
      py += dyTile;
    }
    if ((isV_Collided && (depth_v < depth_h)) || (isV_Collided && isH_Collided)) break;
  }
  if (depth_v < depth_h) {
    castInfo.depth = depth_v + depthGlass_v;
    castInfo.texture = texture_v;
    castInfo.isCollided = isV_Collided;
  } else {
    castInfo.depth = depth_h + depthGlass_h;
    castInfo.texture = texture_h;
    castInfo.isCollided = isH_Collided;
  }
  // tft.drawLine(x, y, x + castInfo.depth * cos_a, y + castInfo.depth * sin_a, tft.color565(100, 255, 255));
  // Serial.print("x: ");
  // Serial.print(x);
  // Serial.print("; y: ");
  // Serial.print(y);
  // Serial.print("; depth: ");
  // Serial.println(castInfo.depth);
}

void RaysCasting(const boolean isClearDisplay) {
  struct {
    byte proect_height;
    col_vec3 texture;
  } castedWalls[NumRays];
  // Init
  const byte xm = (byte(x) >> BitTile) << BitTile;
  const byte ym = (byte(y) >> BitTile) << BitTile;
  float cur_angle = angle - HalfFow;

  // For through the rays
  for (byte ray = 0; ray < NumRays; ray++) {
    // Ray cast
    float sin_a = sin(cur_angle);
    float cos_a = cos(cur_angle);
    if (sin_a == 0) sin_a = 0.000001;
    if (cos_a == 0) cos_a = 0.000001;

    // verticals & horizontals
    RayCast(sin_a, cos_a, x, y, MaxDepthTile);
    const float fishEyeFix = cos(angle - cur_angle);
    const float depth = castInfo.depth * fishEyeFix;

    // Storing the values in the castedWalls array
    castedWalls[ray].proect_height = (PrectCoeff / depth) > Height ? Height : (PrectCoeff / depth);
    castedWalls[ray].texture = castInfo.texture;

    // Updating the angle
    cur_angle += DeltaAngle;
  }

  for (byte ray = 0; ray < NumRays; ray++) {
    const byte proect_height = castedWalls[ray].proect_height;
    const col_vec3 texture = castedWalls[ray].texture;
    const byte oldWallProjectHeightField = (oldWalls[ray].proect_height >> 1);
    const byte proectHeightField = (proect_height >> 1);
    const byte brickY = HeightField - proectHeightField;
    const byte differenceWallsHeight = oldWallProjectHeightField - proectHeightField;
    const bool isWallChanged = oldWalls[ray].texture != texture || isClearDisplay;
    const byte brickX = ray * Scale;

    // fill the voids
    if (oldWalls[ray].proect_height > proect_height) {
      const byte oldBrickY = HeightField - oldWallProjectHeightField;
      for (byte i = 0; i < Scale; i++) {
        tft.drawFastVLine(brickX + i, oldBrickY, differenceWallsHeight, SkyColor);
        tft.drawFastVLine(brickX + i, oldBrickY + oldWallProjectHeightField + proectHeightField, differenceWallsHeight + 1, FlorColor);
      }
    }
    if (isWallChanged || oldWalls[ray].proect_height < proect_height) {
      // Render of new walls
      const unsigned int color = tft.color565(texture.r, texture.g, texture.b);
      const byte oldBrickY = proectHeightField - oldWallProjectHeightField;

      if (isWallChanged) {
        for (byte i = 0; i < Scale; i++) tft.drawFastVLine(brickX + i, brickY, proect_height, color);
      } else {
        for (byte i = 0; i < Scale; i++) {
          tft.drawFastVLine(brickX + i, brickY, oldBrickY, color);
          tft.drawFastVLine(brickX + i, HeightField + oldWallProjectHeightField, oldBrickY, color);
        }
      }
    }
    // Remember the walls
    oldWalls[ray].proect_height = proect_height;
    oldWalls[ray].texture = texture;
  }
}