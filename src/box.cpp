#include "box.hpp"

Box::Box(unsigned short int label, Vec3usi dimensions, Vec3usi coordinates)
    : Container(label, dimensions), packed(false), coordinates(coordinates),
      packedDimensions(dimensions) {}
Box::Box(unsigned short int label, Vec3usi dimensions)
    : Box(label, dimensions, Vec3usi(0, 0, 0)) {}
Box::Box() : Box(0, Vec3usi(0, 0, 0)) {}
Box::Box(BoxInfo boxData) : Box(boxData.label, boxData.dimensions) {}

void Box::setPacked(bool packed) { this->packed = packed; }
bool Box::getPacked() { return packed; }

void Box::setCoordinates(Vec3usi coordinates) {
  this->coordinates = coordinates;
}
Vec3usi Box::getCoordinates() { return coordinates; }

void Box::setPackedDimensions(Vec3usi packedDimensions) {
  this->packedDimensions = packedDimensions;
}
Vec3usi Box::getPackedDimensions() { return packedDimensions; }