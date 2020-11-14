#include <iostream>
using namespace std;
#include "cell.h"
#ifndef WORLD_H
#define WORLD_H
class World {
    protected:
        unique_ptr<unique_ptr<Cell[]>[]> Cells;
    public:
        void newWorld();
};
#endif