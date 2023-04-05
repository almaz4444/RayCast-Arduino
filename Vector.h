class vec2 {
  public:
    vec2() {}
    vec2(int nx, int ny) {
      x = nx;
      y = ny;
    }
    vec2(int component) {
      x = component;
      y = component;
    }
    vec2 operator+(const vec2& other) {
      return vec2(x + other.x, y + other.y);
    }
    vec2 operator+(const int& other) {
      return vec2(x + other, y + other);
    }
    vec2 operator-(const vec2& other) {
      return vec2(x - other.x, y - other.y);
    }
    vec2 operator-(const int& other) {
      return vec2(x - other, y - other);
    }
    vec2 operator-() {
      return vec2(-x, -y);
    }
    vec2 operator*(const vec2& other) {
      return vec2(x * other.x, y * other.y);
    }
    vec2 operator*(const int& other) {
      return vec2(x * other, y * other);
    }
    vec2 operator/(const vec2& other) {
      return vec2(x / other.x, y / other.y);
    }
    vec2 operator/(const int& other) {
      return vec2(x / other, y / other);
    }
    int operator[](const byte& i) {
      return (i == 0)? x : y;
    }
    bool operator==(const vec2& other) {
      return (x == other.x || y == other.y);
    }
    bool operator!=(const vec2& other) {
      return (x != other.x || y != other.y);
    }
    int x = 0;
    int y = 0;
    
    vec2 vecAbs() {
      return vec2(abs(x), abs(y));
    }
};

class vec3 {
  public:
    vec3() {}
    vec3(int nx, int ny, int nz) {
      x = nx;
      y = ny;
      z = nz;
      xy = vec2(x, y);
      xz = vec2(x, z);
      yx = vec2(y, x);
      yz = vec2(y, z);
      zx = vec2(z, x);
      zy = vec2(z, y);
    }
    vec3(int nx, vec2 vector2) {
      x = nx;
      y = vector2.x;
      z = vector2.y;
      xy = vec2(x, y);
      xz = vec2(x, z);
      yx = vec2(y, x);
      yz = vec2(y, z);
      zx = vec2(z, x);
      zy = vec2(z, y);
      xx = vec2(x, x);
    }
    vec3(vec2 vector2, int nx) {
      x = vector2.x;
      y = vector2.y;
      z = nx;
      xy = vec2(x, y);
      xz = vec2(x, z);
      yx = vec2(y, x);
      yz = vec2(y, z);
      zx = vec2(z, x);
      zy = vec2(z, y);
      xx = vec2(x, x);
    }
    vec3(int component) {
      x = component;
      y = component;
      z = component;
      xy = vec2(x, y);
      xz = vec2(x, z);
      yx = vec2(y, x);
      yz = vec2(y, z);
      zx = vec2(z, x);
      zy = vec2(z, y);
      xx = vec2(x, x);
    }
    vec3 operator+(const vec3& other) {
      return vec3(x + other.x, y + other.y, z + other.z);
    }
    vec3 operator+(const int& other) {
      return vec3(x + other, y + other, z + other);
    }
    vec3 operator-(const vec3& other) {
      return vec3(x - other.x, y - other.y, z - other.z);
    }
    vec3 operator-() {
      return vec3(-x, -y, -z);
    }
    vec3 operator-(const int& other) {
      return vec3(x - other, y - other, z - other);
    }
    vec3 operator*(const vec3& other) {
      return vec3(x * other.x, y * other.y, z * other.z);
    }
    vec3 operator*(const int& other) {
      return vec3(x * other, y * other, z * other);
    }
    vec3 operator/(const vec3& other) {
      return vec3(x / other.x, y / other.y, z / other.z);
    }
    vec3 operator/(const int& other) {
      return vec3(x / other, y / other, z / other);
    }
    bool operator==(const vec3& other) {
      return (x == other.x || y == other.y || z == other.x);
    }
    bool operator!=(const vec3& other) {
      return (x != other.x || y != other.y || z != other.x);
    }
    int x, y, z;
    vec2 xy, xz, yx, yz, zx, zy, xx;
    
    vec3 vecAbs() {
      return vec3(abs(x), abs(y), abs(z));
    }
    vec3 yzx() {
      return vec3(y, z, x);
    }
    vec3 zxy() {
      return vec3(z, x, y);
    }
    vec3 xyx() {
      return vec3(x, y, x);
    }
};
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

vec3 operator+(const int& me, const vec3& other) {
  return vec3(me + other.x, me + other.y, me + other.z);
}
vec3 operator-(const int& me, const vec3& other) {
  return vec3(me - other.x, me - other.y, me - other.z);
}
vec3 operator*(const int& me, const vec3& other) {
  return vec3(me * other.x, me * other.y, me * other.z);
}
vec3 operator/(const int& me, const vec3& other) {
  return vec3(me / other.x, me / other.y, me / other.z);
}
vec2 operator+(const int& me, const vec2& other) {
  return vec2(me + other.x, me + other.y);
}
vec2 operator-(const int& me, const vec2& other) {
  return vec2(me - other.x, me - other.y);
}
vec2 operator*(const int& me, const vec2& other) {
  return vec2(me * other.x, me * other.y);
}
vec2 operator/(const int& me, const vec2& other) {
  return vec2(me / other.x, me / other.y);
}

vec3 normalize(const vec3& me) {
  const int k = sqrt(me.x * me.x + me.y * me.y + me.z * me.z);
  return vec3(me.x / k, me.y / k, me.z / k);
}
vec2 normalize(const vec2& me) {
  const int k = sqrt(me.x * me.x + me.y * me.y);
  return vec2(me.x / k, me.y / k);
}

vec3 step(const vec3& me, const vec3& other) {
  vec3 result;
  result.x = (me.x < other.x)? (int)1 : (int)0;
  result.y = (me.y < other.y)? (int)1 : (int)0;
  result.z = (me.z < other.z)? (int)1 : (int)0;
  return result;
}
vec2 step(const vec2& me, const vec2& other) {
  vec2 result;
  result.x = (me.x < other.x)? (int)1 : (int)0;
  result.y = (me.y < other.y)? (int)1 : (int)0;
  return result;
}

int dot(const vec3& me, const vec3& other) {
  return me.x * other.x + me.y * other.y + me.z * other.z;
}
int dot(const vec2& me, const vec2& other) {
  return me.x * other.x + me.y * other.y;
}

vec3 sign(const vec3& me) {
  vec3 result;
  if(me.x > 0) result.x = 1;
  else if(me.x < 0) result.x = -1;
  else result.x = 0;
  if(me.y > 0) result.y = 1;
  else if(me.y < 0) result.y = -1;
  else result.y = 0;
  if(me.z > 0) result.z = 1;
  else if(me.z < 0) result.z = -1;
  else result.z = 0;
  return result;
}
vec2 sign(const vec2& me) {
  vec2 result;
  if(me.x > 0) result.x = 1;
  else if(me.x < 0) result.x = -1;
  else result.x = 0;
  if(me.y > 0) result.y = 1;
  else if(me.y < 0) result.y = -1;
  else result.y = 0;
  return result;
}

vec3 clamp(const vec3& me, const int& minV, const int& maxV) {
  vec3 result;
  if(me.x > maxV) result.x = maxV;
  else if(me.x < minV) result.x = minV;
  else result.x = me.x;
  if(me.y > maxV) result.y = 1;
  else if(me.y < minV) result.y = minV;
  else result.y = me.y;
  if(me.z > maxV) result.z = 1;
  else if(me.z < minV) result.z = minV;
  else result.z = me.z;
  return result;
}
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
vec2 clamp(const vec2& me, const int& minV, const int& maxV) {
  vec2 result;
  if(me.x > maxV) result.x = maxV;
  else if(me.x < minV) result.x = minV;
  else result.x = me.x;
  if(me.y > maxV) result.y = 1;
  else if(me.y < minV) result.y = minV;
  else result.y = me.y;
  return result;
}

vec3 fract(const vec3& me) {
  return vec3(me.x - (int)(int)me.x, me.y - (int)(int)me.y, me.z - (int)(int)me.z);
}
vec2 fract(const vec2& me) {
  return vec2(me.x - (int)(int)me.x, me.y - (int)(int)me.y);
}

vec3 reflect(vec3 lightDir, vec3 normal) {
  return lightDir - 2.0 * dot(normal, lightDir) * normal;
}

vec3 refract(vec3 lightDir, vec3 normal, int n) {
  int cosi = dot(-lightDir, normal);
  int k = 1.0 - n * n * (1.0 - cosi * cosi);
  if (k < 0.0)
      return vec3(0.0);
  else
      return n * lightDir + (n * cosi - sqrt(k)) * normal;
}

vec3 mix(vec3 x, vec3 y, int a) {
  return x * (1.0 - a) + y * a;
}