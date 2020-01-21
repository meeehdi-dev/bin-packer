#include "node.hpp"

Node::Node(Node *previous, Node *next, Vec3usi gap)
    : previous(previous), next(next), gap(gap) {}
Node::Node(Node *previous, Node *next)
    : Node(previous, next, Vec3usi(0, 0, 0)) {}
Node::Node() : Node(nullptr, nullptr) {}