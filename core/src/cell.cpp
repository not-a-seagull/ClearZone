#include "cell.h"

#include <iostream>
#include <vector>
using namespace std;

Cell::Cell() { this->cellBiomeID = Biome; }

Cell::Cell(Biome biome) { this->cellBiomeID = b; }

uint8_t Cell::GetBiome() { return this->cellBiomeID; }
