#pragma once

#include "vec3usi.hpp"

class Node {
public:
  Node *previous, *next;
  Vec3usi gap;

  Node(Node *previous, Node *next, Vec3usi gap);
  Node(Node *previous, Node *next);
  Node();
};