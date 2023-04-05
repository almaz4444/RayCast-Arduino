class Wall {
  public:
    Wall() {}
    Wall(const byte nx, const byte ny, const col_vec3 ncolor, const byte nreflectivity, const byte ntransparency, const bool nisWall) {
      x = nx;
      y = ny;
      color = ncolor;
      reflectivity = nreflectivity;
      transparency = ntransparency;
      isWall = nisWall;
    }

    byte x, y;
    col_vec3 color;
    byte reflectivity, transparency;
    bool isWall;
};