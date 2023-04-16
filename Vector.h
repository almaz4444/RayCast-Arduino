class col_vec3 {
  public:
    col_vec3() {}
    col_vec3(byte nr, byte ng, byte nb) {
      r = nr;
      g = ng;
      b = nb;
    }
    col_vec3(byte component) {
      r = component;
      g = component;
      b = component;
    }
    col_vec3 operator+(const col_vec3& other) {
      return col_vec3(r + other.r, g + other.g, b + other.b);
    }
    col_vec3 operator+(const byte& other) {
      return col_vec3(r + other, g + other, b + other);
    }
    col_vec3 operator-(const col_vec3& other) {
      return col_vec3(r - other.r, g - other.g, b - other.b);
    }
    col_vec3 operator-() {
      return col_vec3(-r, -g, -b);
    }
    col_vec3 operator-(const byte& other) {
      return col_vec3(r - other, g - other, b - other);
    }
    col_vec3 operator*(const col_vec3& other) {
      return col_vec3(r * other.r, g * other.g, b * other.b);
    }
    col_vec3 operator*(const byte& other) {
      return col_vec3(r * other, g * other, b * other);
    }
    col_vec3 operator/(const col_vec3& other) {
      return col_vec3(r / other.r, g / other.g, b / other.b);
    }
    col_vec3 operator/(const byte& other) {
      return col_vec3(r / other, g / other, b / other);
    }
    bool operator==(const col_vec3& other) {
      return (r == other.r || g == other.g || b == other.r);
    }
    bool operator!=(const col_vec3& other) {
      return (r != other.r || g != other.g || b != other.r);
    }
    byte r, g, b;
    
    col_vec3 vecAbs() {
      return col_vec3(abs(r), abs(g), abs(b));
    }
    col_vec3 gbr() {
      return col_vec3(g, b, r);
    }
    col_vec3 brg() {
      return col_vec3(b, r, g);
    }
    col_vec3 rgr() {
      return col_vec3(r, g, r);
    }
};

col_vec3 clamp(const col_vec3& me, const byte& minV, const byte& maxV) {
  col_vec3 result;
  if(me.r > maxV) result.r = maxV;
  else if(me.r < minV) result.r = minV;
  else result.r = me.r;
  if(me.g > maxV) result.g = 1;
  else if(me.g < minV) result.g = minV;
  else result.g = me.g;
  if(me.b > maxV) result.b = 1;
  else if(me.b < minV) result.b = minV;
  else result.b = me.b;
  return result;
}