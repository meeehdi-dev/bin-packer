#include "packer.hpp"
#include "vec3usi.hpp"
#include <algorithm>
#include <climits>
#include <cstdlib>

void Packer::setBoxes(std::vector<BoxInfo> boxesInfo) {
  currentBoxes.clear();

  for (BoxInfo boxInfo : boxesInfo) {
    unsigned int count = boxInfo.count - boxInfo.packed;
    while (count-- > 0) {
      currentBoxes.push_back(Box(boxInfo));
    }
  }
}

//********************************************************
// PERFORMS INITIALIZATIONS
//********************************************************

void Packer::init(Container container) {
  containerVolume =
      container.dimensions.x * container.dimensions.y * container.dimensions.z;
  boxesVolume = 0.0;
  for (unsigned short int i = 0; i < currentBoxes.size(); i++) {
    boxesVolume = boxesVolume + currentBoxes[i].volume;
  }

  head = new Node();

  bestPackedVolume = 0.0;
  isFull = false;
}

//************************************************************
// AFTER FINDING EACH BOX, THE CANDIDATE BOXES AND THE
// CONDITION OF THE LAYER ARE EXAMINED
//************************************************************

void Packer::checkFound(unsigned short int &layersDepth,
                        unsigned short int &previousLayerThickness,
                        unsigned short int &smallestZ) {
  isEvened = false;
  if (boxIndex >= 0) {
    cboxIndex = boxIndex;
    cboxDimensions = boxDimensions;
  } else {
    if ((bboxIndex >= 0) && (layersDepth || ((*smallest).previous == nullptr &&
                                             (*smallest).next == nullptr))) {
      if (!layersDepth) {
        previousLayerThickness = layerThickness;
        smallestZ = (*smallest).gap.z;
      }
      cboxIndex = bboxIndex;
      cboxDimensions = bboxDimensions;
      layersDepth = layersDepth + bboxDimensions.y - layerThickness;
      layerThickness = bboxDimensions.y;
    } else {
      if ((*smallest).previous == nullptr && (*smallest).next == nullptr) {
        isLayerDone = true;
      } else {
        isEvened = true;
        if ((*smallest).previous == nullptr) {
          Node *tmpNode = (*smallest).next;
          (*smallest).gap.x = (*((*smallest).next)).gap.x;
          (*smallest).gap.z = (*((*smallest).next)).gap.z;
          (*smallest).next = (*((*smallest).next)).next;
          if ((*smallest).next) {
            (*((*smallest).next)).previous = smallest;
          }
          delete tmpNode;
        } else if ((*smallest).next == nullptr) {
          (*((*smallest).previous)).next = nullptr;
          (*((*smallest).previous)).gap.x = (*smallest).gap.x;
          delete smallest;
        } else {
          if ((*((*smallest).previous)).gap.z == (*((*smallest).next)).gap.z) {
            (*((*smallest).previous)).next = (*((*smallest).next)).next;
            if ((*((*smallest).next)).next) {
              (*((*((*smallest).next)).next)).previous = (*smallest).previous;
            }
            (*((*smallest).previous)).gap.x = (*((*smallest).next)).gap.x;
            delete (*smallest).next;
            delete smallest;
          } else {
            (*((*smallest).previous)).next = (*smallest).next;
            (*((*smallest).next)).previous = (*smallest).previous;
            if ((*((*smallest).previous)).gap.z < (*((*smallest).next)).gap.z) {
              (*((*smallest).previous)).gap.x = (*smallest).gap.x;
            }
            delete smallest;
          }
        }
      }
    }
  }
}

//**********************************************************************
// ANALYZES EACH UNPACKED BOX TO FIND THE BEST FITTING ONE TO
// THE EMPTY SPACE GIVEN
//**********************************************************************

void Packer::analyzeBox(unsigned short int i, Vec3usi &bfDimensions,
                        Vec3usi &bbfDimensions, short int currentLayerThickness,
                        short int currantGapZ, Vec3usi gap,
                        Vec3usi dimensions) {
  unsigned short int x = dimensions.x;
  unsigned short int y = dimensions.y;
  unsigned short int z = dimensions.z;
  if (x <= gap.x && y <= gap.y && z <= gap.z) {
    if (y <= currentLayerThickness) {
      if ((currentLayerThickness - y < bfDimensions.y) ||
          (currentLayerThickness - y == bfDimensions.y &&
           gap.x - x < bfDimensions.x) ||
          (currentLayerThickness - y == bfDimensions.y &&
           gap.x - x == bfDimensions.x &&
           abs(currantGapZ - z) < bfDimensions.z)) {
        boxDimensions = dimensions;
        bfDimensions =
            Vec3usi(gap.x - x, currentLayerThickness - y, abs(currantGapZ - z));
        boxIndex = i;
      }
    } else {
      if ((y - currentLayerThickness < bbfDimensions.y) ||
          (y - currentLayerThickness == bbfDimensions.y &&
           gap.x - x < bbfDimensions.x) ||
          (y - currentLayerThickness == bbfDimensions.y &&
           gap.x - x == bbfDimensions.x &&
           abs(currantGapZ - z) < bbfDimensions.z)) {
        bboxDimensions = dimensions;
        bbfDimensions =
            Vec3usi(gap.x - x, y - currentLayerThickness, abs(currantGapZ - z));
        bboxIndex = i;
      }
    }
  }
}

//**********************************************************************
// LISTS ALL POSSIBLE LAYER HEIGHTS BY GIVING A WEIGHT VALUE TO
// EACH OF THEM.
//**********************************************************************

void Packer::listLayers() {
  layers.clear();

  for (unsigned short int i = 0; i < currentBoxes.size(); i++) {
    unsigned short int cidX = currentBoxes[i].dimensions.x;
    unsigned short int cidY = currentBoxes[i].dimensions.y;
    unsigned short int cidZ = currentBoxes[i].dimensions.z;
    unsigned short int pcdX = packedContainerDimensions.x;
    unsigned short int pcdY = packedContainerDimensions.y;
    unsigned short int pcdZ = packedContainerDimensions.z;

    for (unsigned short int j = 1; j <= 3; j++) {
      Vec3usi dimensions;

      switch (j) {
      case 1:
        dimensions = Vec3usi(cidX, cidY, cidZ);
        break;
      case 2:
        dimensions = Vec3usi(cidY, cidX, cidZ);
        break;
      case 3:
        dimensions = Vec3usi(cidZ, cidX, cidY);
        break;
      }

      unsigned short int x = dimensions.x;
      unsigned short int y = dimensions.y;
      unsigned short int z = dimensions.z;

      if (x > pcdY || ((y > pcdX || z > pcdZ) && (z > pcdX || y > pcdZ))) {
        continue;
      }
      bool same = false;

      for (unsigned short int k = 0; k < layers.size(); k++) {
        if (x == layers[k].dimension) {
          same = true;
          continue;
        }
      }
      if (same) {
        continue;
      }

      long int layerScore = 0;
      for (unsigned short int k = 0; k < currentBoxes.size(); k++) {
        if (i != k) {
          unsigned short int ckdX = currentBoxes[k].dimensions.x;
          unsigned short int ckdY = currentBoxes[k].dimensions.y;
          unsigned short int ckdZ = currentBoxes[k].dimensions.z;

          unsigned short int diff = abs(x - ckdX);
          if (abs(x - ckdY) < diff) {
            diff = abs(x - ckdY);
          }
          if (abs(x - ckdZ) < diff) {
            diff = abs(x - ckdZ);
          }
          layerScore += diff;
        }
      }
      layers.push_back(Layer(layerScore, x));
    }
  }
}

//********************************************************
// FINDS THE FIRST TO BE PACKED GAP IN THE LAYER EDGE
//********************************************************

void Packer::findSmallest() {
  Node *tmp = head;
  smallest = tmp;
  while ((*tmp).next != nullptr) {
    if ((*((*tmp).next)).gap.z < (*smallest).gap.z) {
      smallest = (*tmp).next;
    }
    tmp = (*tmp).next;
  }
}

//**********************************************************************
// FINDS THE MOST PROPER BOXES BY LOOKING AT ALL SIX POSSIBLE
// ORIENTATIONS, EMPTY SPACE GIVEN, ADJACENT BOXES, AND PALLET LIMITS
//**********************************************************************

void Packer::findBox(short int currentLayerThickness, short int currentGapZ,
                     Vec3usi gap) {
  Vec3usi bfDimensions = Vec3usi(USHRT_MAX, USHRT_MAX, USHRT_MAX);
  Vec3usi bbfDimensions = Vec3usi(USHRT_MAX, USHRT_MAX, USHRT_MAX);
  boxIndex = -1;
  bboxIndex = -1;
  for (unsigned short int i = 0; i < currentBoxes.size(); ++i) {
    if (currentBoxes[i].packed) {
      continue;
    }
    unsigned short int x = currentBoxes[i].dimensions.x;
    unsigned short int y = currentBoxes[i].dimensions.y;
    unsigned short int z = currentBoxes[i].dimensions.z;
    analyzeBox(i, bfDimensions, bbfDimensions, currentLayerThickness,
               currentGapZ, gap, Vec3usi(x, y, z));
    if (x == y && y == z) {
      continue;
    }
    analyzeBox(i, bfDimensions, bbfDimensions, currentLayerThickness,
               currentGapZ, gap, Vec3usi(x, z, y));
    analyzeBox(i, bfDimensions, bbfDimensions, currentLayerThickness,
               currentGapZ, gap, Vec3usi(y, x, z));
    analyzeBox(i, bfDimensions, bbfDimensions, currentLayerThickness,
               currentGapZ, gap, Vec3usi(y, z, x));
    analyzeBox(i, bfDimensions, bbfDimensions, currentLayerThickness,
               currentGapZ, gap, Vec3usi(z, x, y));
    analyzeBox(i, bfDimensions, bbfDimensions, currentLayerThickness,
               currentGapZ, gap, Vec3usi(z, y, x));
  }
}

//*********************************************************************
// TRANSFORMS THE FOUND COORDINATE SYSTEM TO THE ONE ENTERED
// BY THE USER AND WRITES THEM TO THE REPORT FILE
//*********************************************************************

void Packer::packBox() {
  Box &currentBox = currentBoxes[cboxIndex];

  unsigned short int x = currentBox.coordinates.x;
  unsigned short int y = currentBox.coordinates.y;
  unsigned short int z = currentBox.coordinates.z;
  unsigned short int bx = currentBox.packedDimensions.x;
  unsigned short int by = currentBox.packedDimensions.y;
  unsigned short int bz = currentBox.packedDimensions.z;

  switch (bestOrientation) {
  case 1:
    currentBox.coordinates = Vec3usi(x, y, z);
    currentBox.packedDimensions = Vec3usi(bx, by, bz);
    break;
  case 2:
    currentBox.coordinates = Vec3usi(z, y, x);
    currentBox.packedDimensions = Vec3usi(bz, by, bx);
    break;
  case 3:
    currentBox.coordinates = Vec3usi(y, z, x);
    currentBox.packedDimensions = Vec3usi(by, bz, bx);
    break;
  case 4:
    currentBox.coordinates = Vec3usi(y, x, z);
    currentBox.packedDimensions = Vec3usi(by, bx, bz);
    break;
  case 5:
    currentBox.coordinates = Vec3usi(x, z, y);
    currentBox.packedDimensions = Vec3usi(bx, bz, by);
    break;
  case 6:
    currentBox.coordinates = Vec3usi(z, x, y);
    currentBox.packedDimensions = Vec3usi(bz, bx, by);
    break;
  }
}

//********************************************************************
// AFTER PACKING OF EACH BOX, 100% PACKING CONDITION IS CHECKED
//********************************************************************

void Packer::checkVolume(bool best) {
  currentBoxes[cboxIndex].packed = true;
  currentBoxes[cboxIndex].packedDimensions = cboxDimensions;
  packedVolume = packedVolume + currentBoxes[cboxIndex].volume;
  if (best) {
    packBox();
  } else if (packedVolume == containerVolume || packedVolume == boxesVolume) {
    isPacking = false;
    isFull = true;
  }
}

//**********************************************************************
// PACKS THE BOXES FOUND AND ARRANGES ALL VARIABLES AND
// RECORDS PROPERLY
//**********************************************************************

void Packer::packLayer(unsigned short int &layersDepth,
                       unsigned short int &previousLayerThickness,
                       unsigned short int &smallestZ, bool best) {
  unsigned short int lenx, lenz, lpz;

  if (layerThickness == 0) {
    isPacking = false;
    return;
  }

  (*head).gap.x = packedContainerDimensions.x;
  (*head).gap.z = 0;

  while (true) {
    findSmallest();

    if ((*smallest).previous == nullptr && (*smallest).next == nullptr) {
      //*** SITUATION-1: NO BOXES ON THE RIGHT AND LEFT SIDES ***

      lenx = (*smallest).gap.x;
      lpz = remainingContainerZ - (*smallest).gap.z;
      findBox(layerThickness, lpz, Vec3usi(lenx, remainingContainerY, lpz));
      checkFound(layersDepth, previousLayerThickness, smallestZ);

      if (isLayerDone) {
        break;
      }
      if (isEvened) {
        continue;
      }

      currentBoxes[cboxIndex].coordinates =
          Vec3usi(0, packedY, (*smallest).gap.z);

      if (cboxDimensions.x == (*smallest).gap.x) {
        (*smallest).gap.z = (*smallest).gap.z + cboxDimensions.z;
      } else {
        (*smallest).next =
            new Node(smallest, nullptr,
                     Vec3usi((*smallest).gap.x, 0, (*smallest).gap.z));
        (*smallest).gap.x = cboxDimensions.x;
        (*smallest).gap.z = (*smallest).gap.z + cboxDimensions.z;
      }
      checkVolume(best);
    } else if ((*smallest).previous == nullptr) {
      //*** SITUATION-2: NO BOXES ON THE LEFT SIDE ***

      lenx = (*smallest).gap.x;
      lenz = (*((*smallest).next)).gap.z - (*smallest).gap.z;
      lpz = remainingContainerZ - (*smallest).gap.z;
      findBox(layerThickness, lenz, Vec3usi(lenx, remainingContainerY, lpz));
      checkFound(layersDepth, previousLayerThickness, smallestZ);

      if (isLayerDone) {
        break;
      }
      if (isEvened) {
        continue;
      }

      currentBoxes[cboxIndex].coordinates.y = packedY;
      currentBoxes[cboxIndex].coordinates.z = (*smallest).gap.z;
      if (cboxDimensions.x == (*smallest).gap.x) {
        currentBoxes[cboxIndex].coordinates.x = 0;
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).next)).gap.z) {
          (*smallest).gap.z = (*((*smallest).next)).gap.z;
          (*smallest).gap.x = (*((*smallest).next)).gap.x;
          Node *tmpNode = (*smallest).next;
          (*smallest).next = (*((*smallest).next)).next;
          if ((*smallest).next) {
            (*((*smallest).next)).previous = smallest;
          }
          delete tmpNode;
        } else {
          (*smallest).gap.z = (*smallest).gap.z + cboxDimensions.z;
        }
      } else {
        currentBoxes[cboxIndex].coordinates.x =
            (*smallest).gap.x - cboxDimensions.x;
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).next)).gap.z) {
          (*smallest).gap.x = (*smallest).gap.x - cboxDimensions.x;
        } else {
          (*((*smallest).next)).previous = new Node(smallest, (*smallest).next);
          (*smallest).next = (*((*smallest).next)).previous;
          (*((*smallest).next)).gap.x = (*smallest).gap.x;
          (*smallest).gap.x = (*smallest).gap.x - cboxDimensions.x;
          (*((*smallest).next)).gap.z = (*smallest).gap.z + cboxDimensions.z;
        }
      }
      checkVolume(best);
    } else if ((*smallest).next == nullptr) {
      //*** SITUATION-3: NO BOXES ON THE RIGHT SIDE ***

      lenx = (*smallest).gap.x - (*((*smallest).previous)).gap.x;
      lenz = (*((*smallest).previous)).gap.z - (*smallest).gap.z;
      lpz = remainingContainerZ - (*smallest).gap.z;
      findBox(layerThickness, lenz, Vec3usi(lenx, remainingContainerY, lpz));
      checkFound(layersDepth, previousLayerThickness, smallestZ);

      if (isLayerDone) {
        break;
      }
      if (isEvened) {
        continue;
      }

      currentBoxes[cboxIndex].coordinates =
          Vec3usi((*((*smallest).previous)).gap.x, packedY, (*smallest).gap.z);

      if (cboxDimensions.x ==
          (*smallest).gap.x - (*((*smallest).previous)).gap.x) {
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).previous)).gap.z) {
          (*((*smallest).previous)).gap.x = (*smallest).gap.x;
          (*((*smallest).previous)).next = nullptr;
          delete smallest;
        } else {
          (*smallest).gap.z = (*smallest).gap.z + cboxDimensions.z;
        }
      } else {
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).previous)).gap.z) {
          (*((*smallest).previous)).gap.x =
              (*((*smallest).previous)).gap.x + cboxDimensions.x;
        } else {
          (*((*smallest).previous)).next =
              new Node((*smallest).previous, smallest);
          (*smallest).previous = (*((*smallest).previous)).next;
          (*((*smallest).previous)).gap.x =
              (*((*((*smallest).previous)).previous)).gap.x + cboxDimensions.x;
          (*((*smallest).previous)).gap.z =
              (*smallest).gap.z + cboxDimensions.z;
        }
      }
      checkVolume(best);
    } else if ((*((*smallest).previous)).gap.z == (*((*smallest).next)).gap.z) {
      //*** SITUATION-4: THERE ARE BOXES ON BOTH OF THE SIDES ***

      //*** SUBSITUATION-4A: SIDES ARE EQUAL TO EACH OTHER ***

      lenx = (*smallest).gap.x - (*((*smallest).previous)).gap.x;
      lenz = (*((*smallest).previous)).gap.z - (*smallest).gap.z;
      lpz = remainingContainerZ - (*smallest).gap.z;

      findBox(layerThickness, lenz, Vec3usi(lenx, remainingContainerY, lpz));
      checkFound(layersDepth, previousLayerThickness, smallestZ);

      if (isLayerDone) {
        break;
      }
      if (isEvened) {
        continue;
      }

      currentBoxes[cboxIndex].coordinates.y = packedY;
      currentBoxes[cboxIndex].coordinates.z = (*smallest).gap.z;
      if (cboxDimensions.x ==
          (*smallest).gap.x - (*((*smallest).previous)).gap.x) {
        currentBoxes[cboxIndex].coordinates.x = (*((*smallest).previous)).gap.x;
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).next)).gap.z) {
          (*((*smallest).previous)).gap.x = (*((*smallest).next)).gap.x;
          if ((*((*smallest).next)).next) {
            (*((*smallest).previous)).next = (*((*smallest).next)).next;
            (*((*((*smallest).next)).next)).previous = (*smallest).previous;
            delete smallest;
          } else {
            (*((*smallest).previous)).next = nullptr;
            delete smallest;
          }
        } else {
          (*smallest).gap.z = (*smallest).gap.z + cboxDimensions.z;
        }
      } else if ((*((*smallest).previous)).gap.x <
                 packedContainerDimensions.x - (*smallest).gap.x) {
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).previous)).gap.z) {
          (*smallest).gap.x = (*smallest).gap.x - cboxDimensions.x;
          currentBoxes[cboxIndex].coordinates.x =
              (*smallest).gap.x - cboxDimensions.x;
        } else {
          currentBoxes[cboxIndex].coordinates.x =
              (*((*smallest).previous)).gap.x;
          (*((*smallest).previous)).next =
              new Node((*smallest).previous, smallest);
          (*smallest).previous = (*((*smallest).previous)).next;
          (*((*smallest).previous)).gap.x =
              (*((*((*smallest).previous)).previous)).gap.x + cboxDimensions.x;
          (*((*smallest).previous)).gap.z =
              (*smallest).gap.z + cboxDimensions.z;
        }
      } else {
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).previous)).gap.z) {
          (*((*smallest).previous)).gap.x =
              (*((*smallest).previous)).gap.x + cboxDimensions.x;
          currentBoxes[cboxIndex].coordinates.x =
              (*((*smallest).previous)).gap.x;
        } else {
          currentBoxes[cboxIndex].coordinates.x =
              (*smallest).gap.x - cboxDimensions.x;
          (*((*smallest).next)).previous = new Node(smallest, (*smallest).next);
          (*smallest).next = (*((*smallest).next)).previous;
          (*((*smallest).next)).gap.x = (*smallest).gap.x;
          (*((*smallest).next)).gap.z = (*smallest).gap.z + cboxDimensions.z;
          (*smallest).gap.x = (*smallest).gap.x - cboxDimensions.x;
        }
      }
      checkVolume(best);
    } else {
      //*** SUBSITUATION-4B: SIDES ARE NOT EQUAL TO EACH OTHER ***

      lenx = (*smallest).gap.x - (*((*smallest).previous)).gap.x;
      lenz = (*((*smallest).previous)).gap.z - (*smallest).gap.z;
      lpz = remainingContainerZ - (*smallest).gap.z;
      findBox(layerThickness, lenz, Vec3usi(lenx, remainingContainerY, lpz));
      checkFound(layersDepth, previousLayerThickness, smallestZ);

      if (isLayerDone) {
        break;
      }
      if (isEvened) {
        continue;
      }

      currentBoxes[cboxIndex].coordinates =
          Vec3usi((*((*smallest).previous)).gap.x, packedY, (*smallest).gap.z);
      if (cboxDimensions.x ==
          (*smallest).gap.x - (*((*smallest).previous)).gap.x) {
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).previous)).gap.z) {
          (*((*smallest).previous)).gap.x = (*smallest).gap.x;
          (*((*smallest).previous)).next = (*smallest).next;
          (*((*smallest).next)).previous = (*smallest).previous;
          delete smallest;
        } else {
          (*smallest).gap.z = (*smallest).gap.z + cboxDimensions.z;
        }
      } else {
        if ((*smallest).gap.z + cboxDimensions.z ==
            (*((*smallest).previous)).gap.z) {
          (*((*smallest).previous)).gap.x =
              (*((*smallest).previous)).gap.x + cboxDimensions.x;
        } else if ((*smallest).gap.z + cboxDimensions.z ==
                   (*((*smallest).next)).gap.z) {
          currentBoxes[cboxIndex].coordinates.x =
              (*smallest).gap.x - cboxDimensions.x;
          (*smallest).gap.x = (*smallest).gap.x - cboxDimensions.x;
        } else {
          (*((*smallest).previous)).next =
              new Node((*smallest).previous, smallest);
          (*smallest).previous = (*((*smallest).previous)).next;
          (*((*smallest).previous)).gap.x =
              (*((*((*smallest).previous)).previous)).gap.x + cboxDimensions.x;
          (*((*smallest).previous)).gap.z =
              (*smallest).gap.z + cboxDimensions.z;
        }
      }
      checkVolume(best);
    }
  }
}

//**********************************************************************
// FINDS THE MOST PROPER LAYER HIGHT BY LOOKING AT THE UNPACKED
// BOXES AND THE REMAINING EMPTY SPACE AVAILABLE
//**********************************************************************

void Packer::findLayer(short int thickness) {
  long int layerScore, score = LONG_MAX;
  layerThickness = 0;
  for (unsigned short int i = 0; i < currentBoxes.size(); i++) {
    if (currentBoxes[i].packed) {
      continue;
    }
    unsigned short int x = currentBoxes[i].dimensions.x;
    unsigned short int y = currentBoxes[i].dimensions.y;
    unsigned short int z = currentBoxes[i].dimensions.z;
    for (unsigned short int j = 1; j <= 3; j++) {
      Vec3usi dimensions;
      switch (j) {
      case 1:
        dimensions = Vec3usi(x, y, z);
        break;
      case 2:
        dimensions = Vec3usi(y, x, z);
        break;
      case 3:
        dimensions = Vec3usi(z, x, y);
        break;
      }
      layerScore = 0;
      if ((dimensions.x <= thickness) &&
          (((dimensions.y <= packedContainerDimensions.x) &&
            (dimensions.z <= packedContainerDimensions.z)) ||
           ((dimensions.z <= packedContainerDimensions.x) &&
            (dimensions.y <= packedContainerDimensions.z)))) {
        for (unsigned short int k = 0; k < currentBoxes.size(); k++) {
          if (i != k && !currentBoxes[k].packed) {
            unsigned short int diff =
                abs(dimensions.x - currentBoxes[k].dimensions.x);
            if (abs(dimensions.x - currentBoxes[k].dimensions.y) < diff) {
              diff = abs(dimensions.x - currentBoxes[k].dimensions.y);
            }
            if (abs(dimensions.x - currentBoxes[k].dimensions.z) < diff) {
              diff = abs(dimensions.x - currentBoxes[k].dimensions.z);
            }
            layerScore = layerScore + diff;
          }
        }
        if (layerScore < score) {
          score = layerScore;
          layerThickness = dimensions.x;
        }
      }
    }
  }
  if (layerThickness == 0 || layerThickness > remainingContainerY) {
    isPacking = false;
  }
}

void Packer::setOrientation(Container container,
                            unsigned short int orientation) {
  packedContainerDimensions =
      Vec3usi::rotate(container.dimensions, orientation);
}

void Packer::packLayers(bool best) {
  for (Box &currentBox : currentBoxes) {
    currentBox.packed = false;
  }

  do {
    unsigned short int layersDepth = 0;
    unsigned short int previousLayerThickness = 0;
    unsigned short int smallestZ = 0;
    isLayerDone = false;
    packLayer(layersDepth, previousLayerThickness, smallestZ, best);
    packedY = packedY + layerThickness;
    remainingContainerY = packedContainerDimensions.y - packedY;
    if (layersDepth > 0) {
      unsigned short int prepackedy = packedY;
      unsigned short int preremainpy = remainingContainerY;
      remainingContainerY = layerThickness - previousLayerThickness;
      packedY = packedY - layerThickness + previousLayerThickness;
      remainingContainerZ = smallestZ;
      layerThickness = layersDepth;
      isLayerDone = false;
      packLayer(layersDepth, previousLayerThickness, smallestZ, best);
      packedY = prepackedy;
      remainingContainerY = preremainpy;
      remainingContainerZ = packedContainerDimensions.z;
    }
    findLayer(remainingContainerY);
  } while (isPacking);
}

void Packer::sortLayers() {
  listLayers();
  std::sort(layers.begin(), layers.end(), Layer::compare);
}

void Packer::setLayer(unsigned short int index, bool best) {
  packedVolume = 0.0;
  packedY = 0;
  isPacking = true;
  layerThickness = layers[index].dimension;
  remainingContainerY = packedContainerDimensions.y;
  remainingContainerZ = packedContainerDimensions.z;

  packLayers(best);
}

//**********************************************************************
// ITERATIONS ARE DONE AND PARAMETERS OF THE BEST SOLUTION ARE
// FOUND
//**********************************************************************

int Packer::run(Container container) {
  unsigned short int bestLayerIndex = 0;
  for (unsigned short int i = 1; i <= 6; i++) {
    setOrientation(container, i);

    sortLayers();

    for (unsigned short int j = 0; j < layers.size(); j++) {
      setLayer(j, false);

      if (packedVolume > bestPackedVolume) {
        bestPackedVolume = packedVolume;
        bestOrientation = i;
        bestLayerIndex = j;
      }

      if (isFull) {
        break;
      }
    }
    if (isFull) {
      break;
    }
    if ((container.dimensions.x == container.dimensions.y) &&
        (container.dimensions.y == container.dimensions.z)) {
      break;
    }
  }
  return bestLayerIndex;
}

//*******************************************************************
// USING THE PARAMETERS FOUND, PACKS THE BEST SOLUTION FOUND
// AND REPORS TO THE CONSOLE AND TO A TEXT FILE
//*******************************************************************

void Packer::finalize(Container container, unsigned short int layerIndex) {
  setOrientation(container, bestOrientation);

  sortLayers();

  setLayer(layerIndex, true);
}

std::vector<Pack> Packer::pack(std::vector<Container> containers,
                               std::vector<BoxInfo> boxes) {
  std::vector<Pack> packs;

  unsigned int boxesCount = 0;
  for (BoxInfo boxInfo : boxes) {
    boxesCount += boxInfo.count;
  }
  unsigned int packedBoxesCount = 0;

  while (packedBoxesCount < boxesCount) {
    for (int i = 0; i < containers.size(); ++i) {
      Container &container = containers[i];

      std::vector<BoxInfo> unpackedBoxes;
      for (BoxInfo boxInfo : boxes) {
        if (boxInfo.packed < boxInfo.count) {
          unpackedBoxes.push_back(boxInfo);
        }
      }

      setBoxes(unpackedBoxes);

      init(container);
      unsigned short int layerIndex = run(container);
      if (layerIndex >= layers.size()) {
        continue;
      }
      finalize(container, layerIndex);

      Pack pack;
      pack.container = Container(container.label, packedContainerDimensions);
      for (Box currentBox : currentBoxes) {
        if (currentBox.packed) {
          pack.boxes.push_back(currentBox);
        }
      }

      if (pack.boxes.size() == unpackedBoxes.size() ||
          i == containers.size() - 1) {
        packs.push_back(pack);
        for (Box packedBox : pack.boxes) {
          for (BoxInfo &boxInfo : boxes) {
            if (boxInfo.label == packedBox.label) {
              boxInfo.packed++;
            }
          }
        }
        packedBoxesCount += pack.boxes.size();
      }
    }
    if (packedBoxesCount == 0) {
      return packs;
    }
  }

  return packs;
}