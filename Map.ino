void MapInit() {
  // Counting the number of walls
  for(byte i = 0; i < MapRows; i++) {
    for(byte j = 0; j < MapColumns; j++) {
      char cell = StringMap[i][j];
      if(cell != ' ') {
        if(cell == 'B') {
          Map[wallsCount] = Brick(i << BitTile, j << BitTile, 0);
        } else if(cell == 'G') {
          Map[wallsCount] = Brick(i << BitTile, j << BitTile, 1);
        } else if(cell == 'R') {
          Map[wallsCount] = Brick(i << BitTile, j << BitTile, 2);
        } else if(cell == 'W') {
          Map[wallsCount] = Brick(i << BitTile, j << BitTile, 3);
        } else if(cell == '.') {
          x = (i << BitTile) + (Tile >> 1);
          y = (j << BitTile) + (Tile >> 1);
        }
        wallsCount++;
      }
    }
  }
}

char* oldTextures = malloc(sizeof(char) * NumRays);

void SetColorInTexture(byte ray, float cos_a, float sin_a, float depth, byte proect_height, bool V_Min_H, byte wallColor) { // (40, 56) | (32, 48)
  byte localX = byte((x + depth * cos_a) / (Tile / TextureTile)) * (Tile / TextureTile) - byte((x + depth * cos_a) / Tile) * Tile;
  byte localY = byte((y + depth * sin_a) / (Tile / TextureTile)) * (Tile / TextureTile) - byte((y + depth * sin_a) / Tile) * Tile;

  Serial.print(localX);
  Serial.print(" ");
  Serial.println(localY);

  for(byte i = 0; i < TextureTile; i++) {
    char cell = (V_Min_H)? WallTexture[i][localY] : WallTexture[i][localX];
    
    if(oldTextures[ray] != cell) {
      if(cell == 'W') TFTscreen.fill(wallColor, wallColor, wallColor);
      else if (cell == 'B') TFTscreen.fill(wallColor, 0, 0);
      else if (cell == 'G') TFTscreen.fill(0, wallColor, 0);
      else if (cell == 'R') TFTscreen.fill(0, 0, wallColor);

      byte heightWall = proect_height / TextureTile + 1;
      byte yWall = (Height >> 1) - (proect_height >> 1) + i * (proect_height / TextureTile);
      if(yWall + heightWall > Height) heightWall -= (Height >> 1) - (proect_height >> 1) + i * (proect_height / TextureTile) + heightWall - Height;
      else if(yWall < 0) {
        heightWall -= 0 - yWall;
        yWall = 0;
      }
      if(heightWall < 0) break;
      
      TFTscreen.rect(ray * Scale, yWall, Scale, heightWall);
      oldTextures[ray] = cell;
    }
  }
}

// float updateTime;

// void MapUpdate() {
//   for(byte i = 0; i < wallsCount; i++) {
//     if(Map[i].dTime != 0) {
//       if(millis() - Map[i].dTime >= updateTime) {
//         if(Map[i].direction == 'X') Map[i].x += Tile;
//         else if(Map[i].direction == 'x') Map[i].x -= Tile;
//         else if(Map[i].direction == 'Y') Map[i].y += Tile;
//         else if(Map[i].direction == 'y') Map[i].y -= Tile;

//         Serial.print(Map[i].x % Tile);
//         Serial.print(" ");
//         Serial.println(Map[i].y % Tile);

//         if(Map[i].x >= MapRows * Tile - Tile || Map[i].x <= Tile) {
//           if(Map[i].direction == 'X') Map[i].direction = 'x';
//           else if(Map[i].direction == 'x') Map[i].direction = 'X';
//         }
//         if(Map[i].y >= MapColumns * Tile - Tile || Map[i].y <= Tile) {
//           if(Map[i].direction == 'Y') Map[i].direction = 'y';
//           else if(Map[i].direction == 'y') Map[i].direction = 'Y';
//         }
//       }
//     }
//   }
//   updateTime = millis();
// }