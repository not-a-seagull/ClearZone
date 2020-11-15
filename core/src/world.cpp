#include <iostream>
using namespace std;
#include "world.h"
//I'm very tired
void World::newWorld() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            Cells[i][j] = Cell;

        }

    }
}

