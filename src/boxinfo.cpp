#include "boxinfo.hpp"

BoxInfo::BoxInfo(unsigned short int label, Vec3usi dimensions,
                 unsigned short int count)
    : Container(label, dimensions), count(count), packed(false) {}
BoxInfo::BoxInfo(unsigned short int label, Vec3usi dimensions)
    : BoxInfo(label, dimensions, 1) {}
BoxInfo::BoxInfo() : BoxInfo(0, Vec3usi(0, 0, 0)) {}

void BoxInfo::setCount(unsigned short int count) { this->count = count; }
unsigned short int BoxInfo::getCount() { return count; }

void BoxInfo::setPacked(unsigned short int packed) { this->packed = packed; }
unsigned short int BoxInfo::getPacked() { return packed; }