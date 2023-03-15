int oldWalls[NumRays][5];
float oldBlockRay;

void RayCast(bool isClearDisplay) {
  // Init
  byte xm = byte(byte(x) >> BitTile) << BitTile;
  byte ym = byte(byte(y) >> BitTile) << BitTile;
  float cur_angle = angle - HalfFow;
  
  // For through the rays
  for(byte ray = 0; ray < NumRays; ray++) {
    // Ray cast       570 mcs
    float sin_a = sin(cur_angle);
    float cos_a = cos(cur_angle);
    if(sin_a == 0) sin_a = 0.000001;
    if(cos_a == 0) cos_a = 0.000001;

    // verticals & horizontals       160 mcs
    byte px, py;
    int8_t dx, dy;
    byte texture_v, texture_h;
    
    dx = (cos_a >= 0) ? 1 : -1;
    dy = (sin_a >= 0) ? 1 : -1;
    px = xm + (dx == 1 ? Tile : 0);
    py = ym + (dy == 1 ? Tile : 0);
    
    float depth_v, depth_h;
    byte yv, xh;

    bool isV_Collided = false;
    bool isH_Collided = false;
    for(byte i = 0; i < MaxDepth >> BitTile; i++) {
      if(!isV_Collided) {
          depth_v = (px - x) / cos_a;
          yv = y + depth_v * sin_a;
          int8_t c = isInMap((px + dx) >> BitTile, yv >> BitTile);
          if(c != -1) {
            texture_v = c;
            isV_Collided = true;
          }
          px += dx << BitTile;
          if(i + 1 == MaxDepth >> BitTile) texture_v = 255;
          if(depth_v > MaxDepth || Width << BitTile < yv || yv < 0 || px < 0 || px > Height << BitTile) isV_Collided = true;
      }
      if(!isH_Collided) {        
        depth_h = (py - y) / sin_a;
        xh = x + depth_h * cos_a;
        int8_t c = isInMap(xh >> BitTile, (py + dy) >> BitTile);
        if(c != -1) {
          texture_h = c;
          isH_Collided = true;
        }
        py += dy << BitTile;
        if(i + 1 == MaxDepth >> BitTile) texture_h = 255;
        if(depth_h > MaxDepth || (Height << BitTile) < xh || xh < 0 || py > Width << BitTile || py < 0) isH_Collided = true;
      } else if(isV_Collided) break;
    }

    // Projection       ~216 mcs
    float depth;
    byte texture;
    byte wallColor;
    if(depth_v < depth_h) {
      depth = depth_v;
      texture = texture_v;
    } else {
      depth = depth_h;
      texture = texture_h;
    }
    depth *= cos(angle - cur_angle);
    uint16_t proect_height = PrectCoeff / depth;
    if(proect_height > Height) proect_height = Height;
    wallColor = 255 - (depth * (255 / (MaxDepth - (Tile >> 1))));
    if(wallColor < 0) wallColor = 0;
    
    if(Is3D) {
      // fill the voids
      if(oldWalls[ray][2] > proect_height) {
        TFTscreen.fill(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
        TFTscreen.rect(oldWalls[ray][0], oldWalls[ray][1], Scale, (oldWalls[ray][2] >> 1) - (proect_height >> 1));
        TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
        TFTscreen.rect(oldWalls[ray][0], oldWalls[ray][1] + (oldWalls[ray][2] >> 1) + (proect_height >> 1), Scale, (oldWalls[ray][2] >> 1) - (proect_height >> 1) + 1);
      }
      if(oldWalls[ray][2] < proect_height || oldWalls[ray][3] != texture || oldWalls[ray][4] != wallColor || isClearDisplay) {
        // Render of new walls
        bool isTexture = false;
        switch (texture) {
          case 0: TFTscreen.fill(wallColor, 0, 0);
            break;
          case 1: TFTscreen.fill(0, wallColor, 0);
            break;
          case 2: TFTscreen.fill(0, 0, wallColor);
            break;
          case 3: isTexture = true;
            break;
          default: TFTscreen.fill(0, 0, 0);
            break;
        }
        if(isTexture) {
          float currentBlockRay = (depth * cos(angle) - Tile / 2) / (depth * sin(angle));
          if(currentBlockRay != oldBlockRay) {
            SetColorInTexture(depth, cos_a, sin_a, ray, proect_height, texture, wallColor, oldBlockRay != currentBlockRay);
            oldBlockRay = currentBlockRay;
          }
        } else {
          if(oldWalls[ray][3] != texture || oldWalls[ray][4] != wallColor || isClearDisplay) TFTscreen.rect(ray * Scale, (Height >> 1) - (proect_height >> 1), Scale, proect_height);
          else {
            TFTscreen.rect(ray * Scale, (Height >> 1) - (proect_height >> 1), Scale, (proect_height >> 1) - (oldWalls[ray][2] >> 1));
            TFTscreen.rect(ray * Scale, (Height >> 1) + (oldWalls[ray][2] >> 1), Scale, (proect_height >> 1) - (oldWalls[ray][2] >> 1));
          }
        }
      }
      // Remember the walls
      oldWalls[ray][0] = ray * Scale;
      oldWalls[ray][1] = round(Height >> 1) - round(proect_height >> 1);
      oldWalls[ray][2] = proect_height;
      oldWalls[ray][3] = texture;
      oldWalls[ray][4] = wallColor;
    }
    
    cur_angle += DeltaAngle;
  }
}

int8_t isInMap(byte px_x, byte px_y) {
  char cell = StringMap[px_x][px_y];
  if(cell != ' ') {
    if(cell == 'B') return 0;
    else if(cell == 'G') return 1;
    else if(cell == 'R') return 2;
    else if(cell == 'W') return 3;
  }
  return -1;
}