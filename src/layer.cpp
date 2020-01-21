#include "layer.hpp"

Layer::Layer(long int score, unsigned short int dimension)
    : score(score), dimension(dimension) {}
Layer::Layer() : Layer(0, 0) {}