class Wall
{
public:
  Wall() {}
  Wall(const bool nIsError)
  {
    isError = nIsError;
    isWall = false;
  }
  Wall(const uint16_t ncolor, const char **ntexture)
  {
    color = ncolor;
    isWall = true;
    texture = ntexture;
  }

  uint16_t color;
  bool isWall, isError;
  const char **texture;
};