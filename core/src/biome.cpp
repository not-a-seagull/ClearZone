#include <iostream>
#include "biome.h"
using namespace std;

Biome::Biome() {
    this->ID = 0;
    this->color = "Blank";
}

Biome::Biome(string s, int i) {
    this->color = s;
    this->ID = i;
}

