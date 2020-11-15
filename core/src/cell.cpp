#include "cell.h"

#include <iostream>
#include <vector>
using namespace std;

Cell::Cell() { this->biomeID = 0; }
Cell::Cell(uint8_t Biome) { this->biomeID = Biome; }

uint8_t Cell::GetBiome() const { return this->biomeID; }
