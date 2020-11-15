#include <iostream>
#include <vector>

using namespace std;
#ifndef CELL_H
#define CELL_H

#include "entity.h"
#include "item.h"

class Cell {
 protected:
  uint8_t biomeID;

 public:
  Cell();
  Cell(uint8_t biomeID);
  uint8_t GetBiome() const;
};
#endif
