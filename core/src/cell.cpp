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

int Cell::getBiome() const { return this->cellBiomeID; }
void Cell::setBiome(const int b) { this->cellBiomeID = b; }

vector<Entity> Cell::getEnt() const { return this->ent; }
void Cell::setEnt(vector<Entity> e) { this->ent = e; }