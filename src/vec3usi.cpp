#include "vec3usi.hpp"

Vec3usi::Vec3usi(unsigned short int x, unsigned short int y,
                 unsigned short int z)
    : x(x), y(y), z(z) {}
Vec3usi::Vec3usi() : Vec3usi(0, 0, 0) {}

Vec3usi Vec3usi::rotate(Vec3usi vec, unsigned short int orientation) {
  switch (orientation) {
  case 2:
    return Vec3usi(vec.z, vec.y, vec.x);
  case 3:
    return Vec3usi(vec.z, vec.x, vec.y);
  case 4:
    return Vec3usi(vec.y, vec.x, vec.z);
  case 5:
    return Vec3usi(vec.x, vec.z, vec.y);
  case 6:
    return Vec3usi(vec.y, vec.z, vec.x);
  }

  return Vec3usi(vec.x, vec.y, vec.z);
}

void Vec3usi::setX(unsigned short int x) { this->x = x; }
unsigned short int Vec3usi::getX() { return x; }

void Vec3usi::setY(unsigned short int y) { this->y = y; }
unsigned short int Vec3usi::getY() { return y; }

void Vec3usi::setZ(unsigned short int z) { this->z = z; }
unsigned short int Vec3usi::getZ() { return z; }