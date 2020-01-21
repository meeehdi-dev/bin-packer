#pragma once

#include "boxinfo.hpp"
#include "container.hpp"
#include "vec3usi.hpp"

class Box : public Container {
public:
  bool packed;
  Vec3usi coordinates;
  Vec3usi packedDimensions;

  Box(unsigned short int label, Vec3usi dimensions, Vec3usi coordinates);
  Box(unsigned short int label, Vec3usi dimensions);
  Box();
  Box(BoxInfo boxData);

  void setPacked(bool packed);
  bool getPacked();

  void setCoordinates(Vec3usi coordinates);
  Vec3usi getCoordinates();

  void setPackedDimensions(Vec3usi packedDimensions);
  Vec3usi getPackedDimensions();
};