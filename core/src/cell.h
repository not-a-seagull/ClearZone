#include <iostream>
#include <vector>
#include "entity.h"
using namespace std;
#ifndef CELL_H
#define CELL_H
class Cell{
    protected:
        Biome cellBiome;
        vector<Entity> ent;
    public:
        Cell();
        Cell(Biome, vector<Entity>)

        Biome *getBiome() const;
        void setBiome(const Biome&);

        vector<Entity> getEnt() const;
        void setEnt(vector<Entity>);
};
#endif