#pragma once

#include "vec3usi.hpp"

class Container {
public:
  unsigned short int label;
  Vec3usi dimensions;
  unsigned int volume;

  Container(unsigned short int label, Vec3usi dimensions);
  Container();

  void setLabel(unsigned short label);
  unsigned short int getLabel();

  void setDimensions(Vec3usi dimensions);
  Vec3usi getDimensions();

  void setVolume(unsigned int volume);
  unsigned int getVolume();
};