class Wall
{
public:
  Wall() {}
  Wall(const bool n_IsError)
  {
    isError = n_IsError;
    isWall = false;
  }
  Wall(const uint16_t n_color, const bool n_isWall)
  {
    color = n_color;
    isWall = n_isWall;
  }

  uint16_t color;
  bool isWall, isError;
};