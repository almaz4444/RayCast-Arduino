void MapInit() {
  // Counting the number of walls
  for(byte i = 0; i < MapRows; i++) {
    for(byte j = 0; j < StringMap[0].length(); j++) {
      char cell = StringMap[i][j];
      if(cell != ' ') {
        if(cell == 'B') {
          Map[wallsCount] = Brick(i * Tile, j * Tile, 0);
        } else if(cell == 'G') {
          Map[wallsCount] = Brick(i * Tile, j * Tile, 1);
        } else if(cell == 'R') {
          Map[wallsCount] = Brick(i * Tile, j * Tile, 2);
        } else if(cell == 'W') {
          Map[wallsCount] = Brick(i * Tile, j * Tile, 1, 1000, 'X');
        }
        wallsCount++;
      }
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
//         if(Map[i].y >= StringMap[0].length() * Tile - Tile || Map[i].y <= Tile) {
//           if(Map[i].direction == 'Y') Map[i].direction = 'y';
//           else if(Map[i].direction == 'y') Map[i].direction = 'Y';
//         }
//       }
//     }
//   }
//   updateTime = millis();
// }