void MapInit() {
  // Counting the number of walls
  for(byte i = 0; i < MapRows; i++) {
    for(byte j = 0; j < MapColumns; j++) {
      switch (pgm_read_byte(&StringMap[i][j])) {
        case 'B':
          Map[i * MapColumns + j] = Wall(tft.color565(0, 0, 255), true);
          break;
        case 'G':
          Map[i * MapColumns + j] = Wall(tft.color565(0, 255, 0), true);
          break;
        case 'R':
          Map[i * MapColumns + j] = Wall(tft.color565(255, 0, 0), true);
          break;
        case 'W':
          Map[i * MapColumns + j] = Wall(tft.color565(0, 0, 0), true);
          break;
        case '.':
          x = (x == 0)? (i << BitTile) + TileField : x;
          y = (y == 0)? (j << BitTile) + TileField : y;
          break;
      }
    }
  }
}