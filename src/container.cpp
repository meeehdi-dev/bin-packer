#include "container.hpp"

Container::Container(unsigned short int label, Vec3usi dimensions)
    : label(label), dimensions(dimensions) {
  volume = dimensions.x * dimensions.y * dimensions.z;
}
Container::Container() : Container(0, Vec3usi(0, 0, 0)) {}

void Container::setLabel(unsigned short int label) { this->label = label; }
unsigned short int Container::getLabel() { return label; }

void Container::setDimensions(Vec3usi dimensions) {
  this->dimensions = dimensions;
}
Vec3usi Container::getDimensions() { return dimensions; }

void Container::setVolume(unsigned int volume) { this->volume = volume; }
unsigned int Container::getVolume() { return volume; }