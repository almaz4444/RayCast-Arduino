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

byte* oldTextures = malloc(sizeof(byte) * NumRays);
byte startWallRay;

void SetColorInTexture(byte proect_height, byte texture, byte wallColor, int* wall, byte ray) { // (40, 56) | (32, 48)
  // byte localX = byte((x + depth * cos_a) / (Tile / TextureTile)) * (Tile / TextureTile) - byte((x + depth * cos_a) / Tile) * Tile;
  // byte localY = byte((y + depth * sin_a) / (Tile / TextureTile)) * (Tile / TextureTile) - byte((y + depth * sin_a) / Tile) * Tile;

  // Serial.print(localX);
  // Serial.print(" ");
  // Serial.println(localY);
  
  // for(byte i = 0; i < TextureTile; i++) {
  //   char cell = (V_Min_H)? WallTexture[i][localY] : WallTexture[i][localX];
    
  //   if(oldTextures[ray] != cell) {
  //     if(cell == 'W') TFTscreen.fill(wallColor, wallColor, wallColor);
  //     else if (cell == 'B') TFTscreen.fill(wallColor, 0, 0);
  //     else if (cell == 'G') TFTscreen.fill(0, wallColor, 0);
  //     else if (cell == 'R') TFTscreen.fill(0, 0, wallColor);

  //     byte heightWall = proect_height / TextureTile + 1;
  //     byte yWall = (Height >> 1) - (proect_height >> 1) + i * (proect_height / TextureTile);
  //     if(yWall + heightWall > Height) heightWall -= (Height >> 1) - (proect_height >> 1) + i * (proect_height / TextureTile) + heightWall - Height;
  //     else if(yWall < 0) {
  //       heightWall -= 0 - yWall;
  //       yWall = 0;
  //     }
  //     if(heightWall < 0) break;
      
  //     TFTscreen.rect(ray * Scale, yWall, Scale, heightWall);
  //     oldTextures[ray] = cell;
  //   }
  // }




  // if(isNewWall) startWallRay = ray;
  
  // byte blockX = abs(x - byte((x + depth * cos_a) / Tile) * Tile);
  // byte blockY = abs(y - byte((y + depth * sin_a) / Tile) * Tile);
  // float angleInDisplay = acos((depth * cos(angle) - Tile / 2) / (depth * sin(angle)));
  // byte rayStopBrick = round((angleInDisplay + HalfFow - angle) / (Fow / NumRays)) + 1;
  // byte widthBrick = (rayStopBrick - startWallRay);

  // if(oldTextures[ray] != texture) {
  // for (byte yc = 0; yc < 16; yc++) {
  //   TFTscreen.drawRect(ray * Scale,
  //                      (Height >> 1) - (proect_height >> 1) + yc * (proect_height / 16.0),
  //                      Scale * 2,
  //                      proect_height / 16,
  //                      TFTscreen.Color565(Smile[16 * yc + (ray - startWallRay)][0], Smile[16 * yc + (ray - startWallRay)][1], Smile[16 * yc + (ray - startWallRay)][2])
  //                     );
  // }
  // Serial.println(startWallRay);
  // Serial.print(" ");
  // Serial.print(rayStopBrick);
  // Serial.print(" ");
  // Serial.println(widthBrick);
  // TFTscreen.drawCircle(rayStopBrick * Scale, Height / 2, 2, TFTscreen.Color565(0, 0, 0));
  //   oldTextures[ray] = texture;
  // }
  //   TFTscreen.drawBitmap(ray * Scale, (Height >> 1) - (proect_height >> 1), Smile, 16, 16, TFTscreen.Color565(0, wallColor, 0));
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