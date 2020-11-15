#include <iostream>
#include <vector>

using namespace std;
#ifndef CELL_H
#define CELL_H
#include "biome.h"
#include "entity.h"
#include "item.h"

class Cell {
 protected:
  Biome cellBiome;
  int index1 = 0, index2 = 0;

 public:
  Cell();
  Cell(Biome, vector<Entity>);

  int getBiome() const;
  void setBiome(int biomeID);
};
#endif
