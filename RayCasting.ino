byte oldWalls[NumRays][4];

// void RayCast() {
//   float xo, yo = x, y;
//   int xm, ym = mapping(xo, yo);
//   float cur_angle = angle - HalfFow;
//   for(byte ray = 0; ray < NumRays; ray++) {
//     float sin_a = sin(cur_angle);
//     float cos_a = cos(cur_angle);
//     sin_a = (sin_a)? sin_a : 0.000001;
//     cos_a = (cos_a)? cos_a : 0.000001;

//     // verticals
//     float px, dx = (cos_a >= 0)? (xm + Tile, 1) : (xm, -1);
//     float depth_v;
//     for(byte i = 0; i < Width; i += Tile) {
//         depth_v = (px - xo) / cos_a;
//         float py = yo + depth_v * sin_a;
//         if(isInMap(mapping(px + dx, py)[0], mapping(px + dx, py)[1])) break;
//         px += dx * Tile;
//     }

//     // horizontals
//     float py, dy = (sin_a >= 0)? (ym + Tile, 1) : (ym, -1);
//     float depth_h;
//     for(byte i = 0; i < Height; i += Tile) {
//         depth_h = (py - yo) / sin_a;
//         float px = xo + depth_h * cos_a;
//         if(isInMap(mapping(px, py + dy)[0], mapping(px, py + dy)[1])) break;
//         py += dy * Tile;
//     }

//     // projection
//     float depth = (depth_v < depth_h)? depth_v : depth_h;
//     depth *= cos(angle - cur_angle);
//     float proj_height = PrectCoeff / depth;
//     float c = 255 / (1 + depth * depth * 0.00002);
//     // if(oldWalls[ray][0] != ray * Scale || oldWalls[ray][1] != round(Height / 2) - round(proj_height / 2) || oldWalls[ray][2] != Scale || oldWalls[ray][3] != proj_height) {
//     //   TFTscreen.fill(0, 0, 0);
//     //   TFTscreen.rect(oldWalls[ray][0], oldWalls[ray][1], oldWalls[ray][2], oldWalls[ray][3]);
//     // }
  
//     TFTscreen.fill(c, round(c / 2), round(c / 3));
//     TFTscreen.rect(ray * Scale, round(Height / 2) - round(proj_height / 2), Scale, proj_height);
//     cur_angle += DeltaAngle;

//     oldWalls[ray][0] = ray * Scale;
//     oldWalls[ray][1] = round(Height / 2) - round(proj_height / 2);
//     oldWalls[ray][2] = Scale;
//     oldWalls[ray][3] = proj_height;
//   }
    
//   // TFTscreen.fill(SkyColor[0], SkyColor[1], SkyColor[2]);
//   // TFTscreen.rect(0, 0, Width, Height / 2);
//   // TFTscreen.fill(FlorColor[0], FlorColor[1], FlorColor[2]);
//   // TFTscreen.rect(0, Height / 2, Width, Height / 2);

//   TFTscreen.drawLine(MiniMapOffset[0] + xo / Tile * TileInMiniMap,
//                      MiniMapOffset[1] + yo / Tile * TileInMiniMap,
//                      MiniMapOffset[0] + xo / Tile * TileInMiniMap + 5 * cos(angle),
//                      MiniMapOffset[1] + yo / Tile * TileInMiniMap + 5 * sin(angle),
//                      TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  
//   TFTscreen.stroke(FPSColor[0], FPSColor[1], FPSColor[2]);
//   TFTscreen.text(("FPS: " + (String)fps).c_str(), 5, 5);
//   TFTscreen.noStroke();

//   TFTscreen.fill(MiniMapColor[0], MiniMapColor[1], MiniMapColor[2]);
//   for(int i = 0; i < wallsCount; i++) {
//     TFTscreen.rect(MiniMapOffset[0] + Map[i][0] / Tile * TileInMiniMap, MiniMapOffset[1] + Map[i][1] / Tile * TileInMiniMap, TileInMiniMap, TileInMiniMap);
//   }
// }

// int* mapping(float a, float b) {
//   return (round(a / Tile) * Tile, round(b / Tile) * Tile);
// }

bool isInMap(int x, int y) {
  for(byte i = 0; i < wallsCount; i++) {
    if(Map[i][0] == x && Map[i][1] == y) return true;
  }
  return false;
}

void RayCast() {
  TFTscreen.fill(SkyColor[0], SkyColor[1], SkyColor[2]);
  TFTscreen.rect(0, 0, Width, Height / 2);
  TFTscreen.fill(FlorColor[0], FlorColor[1], FlorColor[2]);
  TFTscreen.rect(0, Height / 2, Width, Height / 2);  
  
  float cur_angle = angle - HalfFow;
  float xo = x;
  float yo = y;
  for(byte rays = 0; rays < NumRays; rays++) {
    float sin_a = sin(cur_angle);
    float cos_a = cos(cur_angle);
    for(byte depth = 0; depth < MaxDepth; depth++) {
      float nX = xo + depth * cos_a;
      float nY = yo + depth * sin_a;
      if(isInMap(round(nX / Tile) * Tile, round(nY / Tile) * Tile)) {
        depth *= cos(angle - cur_angle);
        float proect_height = PrectCoeff / depth;
        float color = map(depth, MaxDepth, 0, 0, 255);                
        TFTscreen.fill(0, color, 0);
        TFTscreen.rect(rays * Scale, round(Height / 2) - round(proect_height / 2), Scale, proect_height);
        break;
      }
    }
    cur_angle += DeltaAngle;
  }
  TFTscreen.drawLine(MiniMapOffset[0] + xo / Tile * TileInMiniMap,
                     MiniMapOffset[1] + yo / Tile * TileInMiniMap,
                     MiniMapOffset[0] + x / Tile * TileInMiniMap + 5 * cos(angle),
                     MiniMapOffset[1] + y / Tile * TileInMiniMap + 5 * sin(angle),
                     TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  
  TFTscreen.stroke(FPSColor[0], FPSColor[1], FPSColor[2]);
  TFTscreen.text(("FPS: " + (String)fps).c_str(), 5, 5);
  TFTscreen.noStroke();

  TFTscreen.fill(MiniMapColor[0], MiniMapColor[1], MiniMapColor[2]);
  for(int i = 0; i < wallsCount; i++) {
    TFTscreen.rect(MiniMapOffset[0] + Map[i][0] / Tile * TileInMiniMap, MiniMapOffset[1] + Map[i][1] / Tile * TileInMiniMap, TileInMiniMap, TileInMiniMap);
  }
  TFTscreen.drawLine(MiniMapOffset[0] + xo / Tile * TileInMiniMap,
                     MiniMapOffset[1] + yo / Tile * TileInMiniMap,
                     MiniMapOffset[0] + xo / Tile * TileInMiniMap + 5 * cos(angle),
                     MiniMapOffset[1] + yo / Tile * TileInMiniMap + 5 * sin(angle),
                     TFTscreen.Color565(PlayerLookLineColor[0], PlayerLookLineColor[1], PlayerLookLineColor[2]));
  
  TFTscreen.stroke(FPSColor[0], FPSColor[1], FPSColor[2]);
  TFTscreen.text(("FPS: " + (String)fps).c_str(), 5, 5);
  TFTscreen.noStroke();

  TFTscreen.fill(MiniMapColor[0], MiniMapColor[1], MiniMapColor[2]);
  for(int i = 0; i < wallsCount; i++) {
    TFTscreen.rect(MiniMapOffset[0] + Map[i][0] / Tile * TileInMiniMap, MiniMapOffset[1] + Map[i][1] / Tile * TileInMiniMap, TileInMiniMap, TileInMiniMap);
  }
}