class Wall {
  public:
    Wall() {}
    Wall(const byte nx, const byte ny, const col_vec3 ncolor, const bool nIsGlass, const bool nIsWall) {
      x = nx;
      y = ny;
      color = ncolor;
      isGlass = nIsGlass;
      isWall = nIsWall;
    }

    byte x, y;
    col_vec3 color;
    bool isGlass, isWall;
};