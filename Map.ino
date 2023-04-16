void MapInit() {
  // Counting the number of walls
  for(byte i = 0; i < MapRows; i++) {
    for(byte j = 0; j < MapColumns; j++) {
      const char cell = pgm_read_byte(&StringMap[i][j]);
      switch (cell) {
        case 'B':
          Map[i * MapColumns + j] = Wall(i << BitTile, j << BitTile, col_vec3(0, 0, 255), false, true);
          break;
        case 'G':
          Map[i * MapColumns + j] = Wall(i << BitTile, j << BitTile, col_vec3(0, 255, 0), false, true);
          break;
        case 'R':
          Map[i * MapColumns + j] = Wall(i << BitTile, j << BitTile, col_vec3(255, 0, 0), false, true);
          break;
        case 'W':
          Map[i * MapColumns + j] = Wall(i << BitTile, j << BitTile, col_vec3(0, 0, 0), false, true);
          break;
        case 'O':
          Map[i * MapColumns + j] = Wall(i << BitTile, j << BitTile, col_vec3(255, 255, 255), true, true);
          break;
        case '.':
          x = (x == 0)? (i << BitTile) + TileField : x;
          y = (y == 0)? (j << BitTile) + TileField : y;
          break;
      }
    }
  }
}