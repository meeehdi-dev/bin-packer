#pragma once

#include "container.hpp"

class BoxInfo : public Container {
public:
  unsigned short int count;
  unsigned short int packed;

  BoxInfo(unsigned short int label, Vec3usi dimensions,
          unsigned short int count);
  BoxInfo(unsigned short int label, Vec3usi dimensions);
  BoxInfo();

  void setCount(unsigned short int count);
  unsigned short int getCount();

  void setPacked(unsigned short int packed);
  unsigned short int getPacked();
};