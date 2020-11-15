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
  int8 biomeID;

 public:
  Cell(int8 biomeID);
}
#endif
