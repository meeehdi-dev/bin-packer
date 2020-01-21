#include "boxinfo.hpp"
#include "container.hpp"
#include "pack.hpp"
#include "packer.hpp"
#include "vec3usi.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "no input\n";
    return 0;
  }

  std::string filename = argv[1];

  std::ifstream input(filename);
  std::string line;
  std::vector<Container> containers;
  std::vector<BoxInfo> boxesInfo;
  bool boxes = false;
  while (std::getline(input, line)) {
    if (line.compare("") == 0) {
      boxes = true;
      continue;
    }
    std::istringstream iss(line);
    unsigned short int x, y, z, n;
    if (boxes == false) {
      if (!(iss >> x >> y >> z)) {
        continue;
      }
      containers.push_back(Container(containers.size() + 1, Vec3usi(x, y, z)));
    } else {
      if (!(iss >> x >> y >> z >> n)) {
        continue;
      }
      boxesInfo.push_back(BoxInfo(boxesInfo.size() + 1, Vec3usi(x, y, z), n));
    }
  }

  Packer packer;
  std::vector<Pack> packs = packer.pack(containers, boxesInfo);

  std::ofstream output(filename + ".out");
  for (Pack pack : packs) {
    output << pack.container.dimensions.x << " " << pack.container.dimensions.y << " " << pack.container.dimensions.z << "\n\n";
    for (Box box : pack.boxes) {
      output << box.label << " " << box.coordinates.x << " " << box.coordinates.y << " " << box.coordinates.z << " ";
      output << box.packedDimensions.x << " " << box.packedDimensions.y << " " << box.packedDimensions.z << "\n";
    }
    output << "\n";
  }
  output.close();

  return 0;
}
