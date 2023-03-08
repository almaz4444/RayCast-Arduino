float oldWalls[NumRays][4];

void RayCast() {
  // Init
  float xo = x;
  float yo = y;
  int xm = ((int)(xo / Tile)) * Tile;
  int ym = ((int)(yo / Tile)) * Tile;
  float cur_angle = angle - HalfFow;
  
  // For through the rays
  for(byte ray = 0; ray < NumRays; ray++) {
    float sin_a = sin(cur_angle);
    float cos_a = cos(cur_angle);
    if(sin_a == 0) sin_a = 0.000001;
    if(cos_a == 0) cos_a = 0.000001;

    // verticals
    float dx, px;
    byte texture_v;
    if(cos_a >= 0) {
      px = xm + Tile;
      dx = 1;
    } else {
      px = xm;
      dx = -1;
    }
    float depth_v, yv;
    bool isMax_v = false;
    for(byte i = 0; i < MaxDepth; i += Tile) {
        depth_v = (px - xo) / cos_a;
        yv = yo + depth_v * sin_a;
        int8_t tile_v = isInMap(((int)((px + dx) / Tile)) * Tile, ((int)(yv / Tile)) * Tile);
        if(tile_v != -1) {
          texture_v = Map[tile_v].color;
          break;
        }
        px += dx * Tile;
        if(i + 1 == MaxDepth) isMax_v = true;
    }
    
    // horizontals
    float py, dy;
    byte texture_h;
    if(sin_a >= 0) {
      py = ym + Tile;
      dy = 1;
    } else {
      py = ym;
      dy = -1;
    }
    float depth_h, xh;
    bool isMax_h;
    for(byte i = 0; i < MaxDepth; i += Tile) {
        depth_h = (py - yo) / sin_a;
        xh = xo + depth_h * cos_a;
        int8_t tile_h = isInMap(((int)(xh / Tile)) * Tile, ((int)((py + dy) / Tile)) * Tile);
        if(tile_h != -1) {
          texture_h = Map[tile_h].color;
          break;
        }
        py += dy * Tile;
        if(i + 1 == MaxDepth) isMax_h = true;
    }

    // projection
    float depth, offset;
    byte texture;
    float wallColor = -1;
    if(depth_v < depth_h) {
      depth = depth_v;
      offset = yv;
      texture = texture_v;
      if(isMax_v) wallColor = 0;
    }
    else {
      depth = depth_h;
      offset = xh;
      texture = texture_h;
      if(isMax_h) wallColor = 0;
    }
    depth *= cos(angle - cur_angle);
    if(depth < 0.00001) depth = 0.00001;
    offset = ((int)offset) % Tile;
    int proect_height = (int)(PrectCoeff / depth);
    if(proect_height > 2 * Height) proect_height = 2 * Height;
    float c = 255 / (1 + depth * depth * 0.00002);
    if(wallColor == -1) wallColor = map(depth, MaxDepth, 0, 0, 255);
    
    if(Is3D && (
      oldWalls[ray][0] != ray * Scale ||
      oldWalls[ray][1] != round(Height / 2) - round(proect_height / 2) ||
      oldWalls[ray][2] != Scale ||
      oldWalls[ray][3] != proect_height
    )) {
      // Computing voidColor
      byte voidColor[4];
      if(oldWalls[ray][1] < Height / 2) {
        voidColor[0] = SkyColor3D[0];
        voidColor[1] = SkyColor3D[1];
        voidColor[2] = SkyColor3D[2];
      } else {
        voidColor[0] = FlorColor3D[0];
        voidColor[1] = FlorColor3D[1];
        voidColor[2] = FlorColor3D[2];
      }
      
      // We fill the voids
      TFTscreen.fill(voidColor[0], voidColor[1], voidColor[2]);
      TFTscreen.rect(oldWalls[ray][0], oldWalls[ray][1], oldWalls[ray][2], oldWalls[ray][3]);

      // if the filled void is below half the screen, fill with earth (voidColor correction)
      if(oldWalls[ray][1] + oldWalls[ray][3] > Height / 2) {
        TFTscreen.fill(FlorColor3D[0], FlorColor3D[1], FlorColor3D[2]);
        TFTscreen.rect(oldWalls[ray][0], round(Height / 2), oldWalls[ray][2], oldWalls[ray][3]);
      }
      
      // Render of new walls
      if(texture == 0) TFTscreen.fill(wallColor, 0, 0);
      else if(texture == 1) TFTscreen.fill(0, wallColor, 0);
      else if(texture == 2) TFTscreen.fill(0, 0, wallColor);
      TFTscreen.rect(ray * Scale, round(Height / 2) - round(proect_height / 2), Scale, proect_height);
    } else if(!Is3D && (oldWalls[ray][0] != xo || oldWalls[ray][1] != yo || oldWalls[ray][2] != xo + depth * cos_a || oldWalls[ray][3] != yo + depth * sin_a)) {
      // Look line
      TFTscreen.drawLine(oldWalls[ray][0] / Tile * (Width / MapRows), oldWalls[ray][1] / Tile * (Width / MapRows), oldWalls[ray][2] / Tile * (Width / MapRows), oldWalls[ray][3] / Tile * (Width / MapRows), TFTscreen.Color565(0, 0, 0));
      TFTscreen.drawLine(xo / Tile * (Width / MapRows), yo / Tile * (Width / MapRows), (xo + depth * cos_a) / Tile * (Width / MapRows), (yo + depth * sin_a) / Tile * (Width / MapRows), TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
    }

    // Remember the walls
    if(Is3D) {
      oldWalls[ray][0] = ray * Scale;
      oldWalls[ray][1] = round(Height / 2) - round(proect_height / 2);
      oldWalls[ray][2] = Scale;
      oldWalls[ray][3] = proect_height;
    } else {
      oldWalls[ray][0] = xo;
      oldWalls[ray][1] = yo;
      oldWalls[ray][2] = xo + depth * cos(cur_angle);
      oldWalls[ray][3] = yo + depth * sin(cur_angle);
    }
    
    cur_angle += DeltaAngle;
  }
}

int8_t isInMap(int x, int y) {
  for(byte i = 0; i < wallsCount; i++) {
    if(Map[i].x == x && Map[i].y == y) return i;
  }
  return -1;
}