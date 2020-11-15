#include <iostream>
#include <vector>
#include "cell.h"
using namespace std;

Cell::Cell() {
    this->cellBiomeID = Biome;
}

Cell::Cell(Biome b, vector<Entity> e) {
    this->ent = e;
    this->cellBiomeID = b;
}

int Cell::getIndexX() const { return this->index1; }
int Cell::getIndexY() const { return this->index2; } 
