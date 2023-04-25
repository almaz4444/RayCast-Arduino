class Wall {
  public:
    Wall() {}
    Wall(const bool nIsError) {
      isError = nIsError;
      isWall = false;
    }
    Wall(const uint16_t ncolor, const bool nIsWall) {
      color = ncolor;
      isWall = nIsWall;
    }

    uint16_t color;
    bool isWall, isError;
};