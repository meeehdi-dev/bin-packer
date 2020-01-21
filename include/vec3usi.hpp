#pragma once

class Vec3usi {
public:
  unsigned short int x;
  unsigned short int y;
  unsigned short int z;

  Vec3usi(unsigned short int x, unsigned short int y, unsigned short int z);
  Vec3usi();

  static Vec3usi rotate(Vec3usi vec, unsigned short int orientation);

  void setX(unsigned short int x);
  unsigned short int getX();

  void setY(unsigned short int y);
  unsigned short int getY();

  void setZ(unsigned short int z);
  unsigned short int getZ();
};