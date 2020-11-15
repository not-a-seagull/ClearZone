#include <iostream>
#include <vector>

using namespace std;
#ifndef CELL_H
#define CELL_H
#include "biome.h"

class Cell {
 protected:
  int8 biomeID;

 public:
  Cell(int8 biomeID);
#endif
