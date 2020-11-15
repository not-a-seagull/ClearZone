#include <iostream>
using namespace std;

#ifndef WORLD_H
#define WORLD_H
#include "cell.h"
class World {
    protected:
        unique_ptr<unique_ptr<Cell[]>[]> Cells;
    public:
        void newWorld();
};
#endif