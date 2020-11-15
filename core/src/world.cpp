#include <iostream>
using namespace std;
#include "world.h"
//I'm very tired
void World::World(int worldSize) {
    Cell Cell;
    for (int i = 0; i < worldSize; i++) {
        for (int j = 0; j < worldSize; j++) {
            Cell.SetBiome((i * j) % biomeList.getLength());
            Cells[i][j] = Cell;
        }

    }
}

