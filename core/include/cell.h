#include <iostream>
#include <vector>

using namespace std;
#ifndef CELL_H
#define CELL_H
#include "entity.h"
#include "biome.h"
class Cell{
    protected:
        int cellBiomeID;
        vector<Entity> ent;
    public:
        Cell();
        Cell(Biome, vector<Entity>)

        int getBiome() const;
        void setBiome(int biomeID);

        vector<Entity> getEnt() const;
        void setEnt(vector<Entity>);
};
#endif