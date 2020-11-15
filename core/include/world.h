#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <iostream>
using namespace std;
#include "cell.h"


class World {
    protected:
        unique_ptr<unique_ptr<Cell[]>[]> Cells;
    public:
        void newWorld();
};
#endif
