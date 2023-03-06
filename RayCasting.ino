float oldWalls[NumRays][4];

void RayCast() {
  if(NewOptionRender) { /* ______________Option 1______________ */
    // Init
    float xo = x;
    float yo = y;
    int xm = round(xo / Tile) * Tile;
    int ym = round(yo / Tile) * Tile;
    float cur_angle = angle - HalfFow;
    
    // For through the rays
    for(byte ray = 0; ray < NumRays; ray++) {
      float sin_a = sin(cur_angle);
      float cos_a = cos(cur_angle);
      sin_a = (sin_a)? sin_a : 0.000001;
      cos_a = (cos_a)? cos_a : 0.000001;

      // verticals
      float px, dx;
      if(cos_a >= 0) {
        px = xm + Tile;
        dx = 1;
      } else {
        px = xm;
        dx = -1;
      }
      float depth_v;
      for(byte i = 0; i < Width; i += Tile) {
          depth_v = (px - xo) / cos_a;
          float yv = yo + depth_v * sin_a;
          if(isInMap(((int)(px + dx)) / Tile * Tile, ((int)(yv / Tile)) * Tile)) break;
          px += dx * Tile;
      }

      // horizontals
      float py, dy;
      if(sin_a >= 0) {
        py = ym + Tile;
        dy = 1;
      } else {
        py = ym;
        dy = -1;
      }
      float depth_h;
      for(byte i = 0; i < Height; i += Tile) {
          depth_h = (py - yo) / sin_a;
          float xh = xo + depth_h * cos_a;
          if(isInMap(((int)(xh / Tile)) * Tile, ((int)(py + dy)) / Tile * Tile)) break;
          py += dy * Tile;
      }

      // projection
      float depth;
      if(depth_v < depth_h) depth = depth_v;
      else depth = depth_h;
      depth *= cos(angle - cur_angle);
      float proect_height = PrectCoeff / depth;
      float c = 255 / (1 + depth * depth * 0.00002);
      float wallColor = map(depth, MaxDepth, 0, 0, 255);

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
        TFTscreen.fill(0, wallColor, 0);
        TFTscreen.rect(ray * Scale, round(Height / 2) - round(proect_height / 2), Scale, proect_height);
      } else if(!Is3D && (oldWalls[ray][0] != xo || oldWalls[ray][1] != yo || oldWalls[ray][2] != xo + depth * cos(cur_angle) || oldWalls[ray][3] != yo + depth * sin(cur_angle))) {
        // Look line
        TFTscreen.drawLine(oldWalls[ray][0], oldWalls[ray][1], oldWalls[ray][2], oldWalls[ray][3], TFTscreen.Color565(0, 0, 0));
        TFTscreen.drawLine(xo, yo, xo + depth * cos(cur_angle), yo + depth * sin(cur_angle), TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
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
  } else { /* ______________Option 2______________ */
    // Init
    float cur_angle = angle - HalfFow;
    float xo = x;
    float yo = y;
    // For through the rays
    for(byte ray = 0; ray < NumRays; ray++) {
      float sin_a = sin(cur_angle);
      float cos_a = cos(cur_angle);
      // For the distance
      for(byte depth = 0; depth < MaxDepth; depth++) {
        float nX = xo + depth * cos_a;
        float nY = yo + depth * sin_a;
        // Did the beam hit the wall?
        if(isInMap(((int)(nX / Tile)) * Tile, ((int)(nY / Tile)) * Tile)) {
          // Computing the projection
          depth *= cos(angle - cur_angle);
          float proect_height = PrectCoeff / depth;
          float wallColor = map(depth, MaxDepth, 0, 0, 255);

          // If the picture on the screen has changed
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
            TFTscreen.fill(0, wallColor, 0);
            TFTscreen.rect(ray * Scale, round(Height / 2) - round(proect_height / 2), Scale, proect_height);
          } else  if(!Is3D && (oldWalls[ray][0] != xo ||
                    oldWalls[ray][1] != yo ||
                    oldWalls[ray][2] != xo + depth * cos(cur_angle) ||
                    oldWalls[ray][3] != yo + depth * sin(cur_angle)
                  )) {
                    // Look line
                    TFTscreen.drawLine(oldWalls[ray][0], oldWalls[ray][1], oldWalls[ray][2], oldWalls[ray][3], TFTscreen.Color565(0, 0, 0));
                    TFTscreen.drawLine(xo, yo, xo + depth * cos(cur_angle), yo + depth * sin(cur_angle), TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
                  }
          if(Is3D) {
            // Remember the walls
            oldWalls[ray][0] = ray * Scale;
            oldWalls[ray][1] = round(Height / 2) - round(proect_height / 2);
            oldWalls[ray][2] = Scale;
            oldWalls[ray][3] = proect_height;
          } else {
            // Remember the walls
            oldWalls[ray][0] = xo;
            oldWalls[ray][1] = yo;
            oldWalls[ray][2] = xo + depth * cos(cur_angle);
            oldWalls[ray][3] = yo + depth * sin(cur_angle);
          }
          break;
        }
      }
      cur_angle += DeltaAngle;
    }
  }
}

bool isInMap(int x, int y) {
  for(byte i = 0; i < wallsCount; i++) {
    if(Map[i][0] == x && Map[i][1] == y) return true;
  }
  return false;
}