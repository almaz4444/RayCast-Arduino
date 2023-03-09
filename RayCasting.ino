int oldWalls[NumRays][3];

void RayCast(bool isClearDisplay) {
  // Init
  byte xm = ((int)(x / Tile)) * Tile;
  byte ym = ((int)(y / Tile)) * Tile;
  float cur_angle = angle - HalfFow;
  
  // For through the rays
  for(byte ray = 0; ray < NumRays; ray++) {
    float sin_a = sin(cur_angle);
    float cos_a = cos(cur_angle);
    if(sin_a == 0) sin_a = 0.000001;
    if(cos_a == 0) cos_a = 0.000001;

    // verticals
    int px;
    int8_t dx;
    byte texture_v;
    if(cos_a >= 0) {
      px = xm + Tile;
      dx = 1;
    } else {
      px = xm;
      dx = -1;
    }
    float depth_v, yv;
    for(byte i = 0; i < MaxDepth; i += Tile) {
        depth_v = (px - x) / cos_a;
        yv = y + depth_v * sin_a;
        int tile_v = isInMap(byte((px + dx) / Tile) * Tile, byte(yv / Tile) * Tile);
        if(tile_v != -1) {
          texture_v = Map[tile_v].color;
          break;
        }
        px += dx * Tile;
        if(i + Tile == MaxDepth) texture_v = 3;
    }
    
    // horizontals
    int py;
    int8_t dy;
    byte texture_h;
    if(sin_a >= 0) {
      py = ym + Tile;
      dy = 1;
    } else {
      py = ym;
      dy = -1;
    }
    float depth_h, xh;
    for(byte i = 0; i < MaxDepth; i += Tile) {
        depth_h = (py - y) / sin_a;
        xh = x + depth_h * cos_a;
        int tile_h = isInMap(byte(xh / Tile) * Tile, byte((py + dy) / Tile) * Tile);
        if(tile_h != -1) {
          texture_h = Map[tile_h].color;
          break;
        }
        py += dy * Tile;
        if(i + Tile == MaxDepth) texture_h = 3;
        if(depth_h > depth_v) break;
    }

    // Projection
    float depth;
    byte texture, wallColor;
    if(depth_v < depth_h) {
      depth = depth_v;
      texture = texture_v;
    }
    else {
      depth = depth_h;
      texture = texture_h;
    }
    depth *= cos(angle - cur_angle);
    if(depth < 0.00001) depth = 0.00001;
    int proect_height = (int)(PrectCoeff / depth);
    if(proect_height > 2 * Height) proect_height = 2 * Height;
    wallColor = map(depth, MaxDepth, 0, 0, 255);
    
    if(Is3D && (
      oldWalls[ray][0] != ray * Scale ||
      oldWalls[ray][1] != round(Height / 2) - round(proect_height / 2) ||
      oldWalls[ray][2] != proect_height ||
      isClearDisplay
    )) {
      // We fill the voids
      if(oldWalls[ray][1] < Height / 2) TFTscreen.fill(SkyColor3D[0], SkyColor3D[1], SkyColor3D[2]);
      else TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
      TFTscreen.rect(oldWalls[ray][0], oldWalls[ray][1], Scale, oldWalls[ray][2]);

      // if the filled void is below half the screen, fill with ground (voidColor correction)
      if(oldWalls[ray][1] + oldWalls[ray][2] > Height / 2) {
        TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
        TFTscreen.rect(oldWalls[ray][0], round(Height / 2), Scale, oldWalls[ray][2]);
      }
      
      // Render of new walls
      if(texture == 0) TFTscreen.fill(wallColor, 0, 0);
      else if(texture == 1) TFTscreen.fill(0, wallColor, 0);
      else if(texture == 2) TFTscreen.fill(0, 0, wallColor);
      else if(texture == 3) TFTscreen.fill(0, 0, 0);
      TFTscreen.rect(ray * Scale, round(Height / 2) - round(proect_height / 2), Scale, proect_height);
      
      // Remember the walls
      oldWalls[ray][0] = ray * Scale;
      oldWalls[ray][1] = round(Height / 2) - round(proect_height / 2);
      oldWalls[ray][2] = proect_height;
    }
    
    cur_angle += DeltaAngle;
  }
}

int isInMap(byte x, byte y) {
  byte left = 0;
  byte right = wallsCount - 1;
  byte mid;
  
  while (left <= right) {
    mid = (left + right) / 2;
    
    byte mTileX = byte(Map[mid].x / Tile) * Tile;
    byte mTileY = byte(Map[mid].y / Tile) * Tile;
    
    if (mTileX == x && mTileY == y) return mid;
    if((mTileX == x && mTileY > y) || mTileX > x) right = mid - 1;
    else left = mid + 1;
  }
  return -1;
}