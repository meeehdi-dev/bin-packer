#pragma once

#include "box.hpp"
#include "container.hpp"
#include <vector>

class Pack {
public:
  Container container;
  std::vector<Box> boxes;

  Pack();
  Pack(Container container, std::vector<Box> boxes);

  void setContainer(Container container);
  Container getContainer();

  void setBoxes(std::vector<Box> boxes);
  std::vector<Box> getBoxes();
};