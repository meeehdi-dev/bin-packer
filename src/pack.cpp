#include "pack.hpp"

Pack::Pack() {}
Pack::Pack(Container container, std::vector<Box> boxes)
    : container(container), boxes(boxes) {}

void Pack::setContainer(Container container) { this->container = container; }
Container Pack::getContainer() { return container; }

void Pack::setBoxes(std::vector<Box> boxes) { this->boxes = boxes; }
std::vector<Box> Pack::getBoxes() { return boxes; }