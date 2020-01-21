#include "box.hpp"
#include "boxinfo.hpp"
#include "container.hpp"
#include "pack.hpp"
#include "packer.hpp"
#include "vec3usi.hpp"
#include <vector>

#include "nbind/nbind.h"

NBIND_CLASS(Packer) {
  construct<>();

  method(pack);
}

NBIND_CLASS(Vec3usi) {
  construct<>();
  construct<unsigned short int, unsigned short int, unsigned short int>();

  getset(getX, setX);
  getset(getY, setY);
  getset(getZ, setZ);
}

NBIND_CLASS(Container) {
  construct<>();
  construct<unsigned short int, Vec3usi>();

  getset(getLabel, setLabel);
  getset(getDimensions, setDimensions);
  getset(getVolume, setVolume);
}

NBIND_CLASS(Box) {
  inherit(Container);
  construct<>();
  construct<unsigned short int, Vec3usi, Vec3usi>();
  construct<unsigned short int, Vec3usi>();

  getset(getPacked, setPacked);
  getset(getCoordinates, setCoordinates);
  getset(getPackedDimensions, setPackedDimensions);
}

NBIND_CLASS(BoxInfo) {
  inherit(Container);
  construct<>();
  construct<unsigned short int, Vec3usi>();
  construct<unsigned short int, Vec3usi, unsigned short int>();

  getset(getCount, setCount);
  getset(getPacked, setPacked);
}

NBIND_CLASS(Pack) {
  construct<>();
  construct<Container, std::vector<Box>>();

  getset(getContainer, setContainer);
  getset(getBoxes, setBoxes);
}
