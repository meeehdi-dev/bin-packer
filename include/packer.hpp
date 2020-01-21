#pragma once

#include "box.hpp"
#include "boxinfo.hpp"
#include "layer.hpp"
#include "node.hpp"
#include "pack.hpp"
#include "vec3usi.hpp"
#include <vector>

class Packer {
public:
  bool isPacking;
  bool isLayerDone;
  bool isEvened;
  bool isFull;

  short int boxIndex;
  Vec3usi boxDimensions;
  short int bboxIndex;
  Vec3usi bboxDimensions;
  short int cboxIndex;
  Vec3usi cboxDimensions;

  unsigned short int bestOrientation;

  Vec3usi packedContainerDimensions;

  unsigned short int layerThickness;
  unsigned short int remainingContainerY, remainingContainerZ;
  unsigned short int packedY;

  float packedVolume;
  float bestPackedVolume;
  float containerVolume;
  float boxesVolume;

  std::vector<Box> currentBoxes;
  std::vector<Layer> layers;

  Node *head, *smallest;

  void setBoxes(std::vector<BoxInfo> boxesInfo);
  void init(Container container);
  void checkFound(unsigned short int &layersDepth,
                  unsigned short int &previousLayerThickness,
                  unsigned short int &smallestZ);
  void analyzeBox(unsigned short int i, Vec3usi &bfDimensions,
                  Vec3usi &bbfDimensions, short int currentLayerThickness,
                  short int currantGapZ, Vec3usi gap, Vec3usi dimensions);
  void listLayers();
  void findSmallest();
  void findBox(short int currentLayerThickness, short int currentGapZ,
               Vec3usi gap);
  void packBox();
  void checkVolume(bool best);
  void packLayer(unsigned short int &layersDepth,
                 unsigned short int &previousLayerThickness,
                 unsigned short int &smallestZ, bool best);
  void findLayer(short int thickness);
  Vec3usi setOrientation(Vec3usi dimensions, unsigned short int orientation);
  void setOrientation(Container container, unsigned short int orientation);
  void packLayers(bool best);
  void sortLayers();
  void setLayer(unsigned short int index, bool best);
  int run(Container container);
  void finalize(Container container, unsigned short int layerIndex);
  std::vector<Pack> pack(std::vector<Container> containers,
                         std::vector<BoxInfo> boxes);
};