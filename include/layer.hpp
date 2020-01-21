#pragma once

class Layer {
public:
  long int score;
  unsigned short int dimension;

  Layer(long int score, unsigned short int dimension);
  Layer();

  static bool compare(Layer i, Layer j) { return i.score < j.score; }
};
