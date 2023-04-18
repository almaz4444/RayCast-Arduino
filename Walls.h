class Wall {
  public:
    Wall() {}
    Wall(const bool nIsError) {
      isWall = false;
      isError = nIsError;
    }
    Wall(const uint ncolor, const bool nIsWall) {
      color = ncolor;
      isWall = nIsWall;
    }

    uint color;
    bool isWall, isError;
};