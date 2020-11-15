#include <iostream>
#include "biome.h"
using namespace std;

BiomeList::AddBiome(char biomeColor) {
    biomes.push_back(biomeColor);
}

BiomeList::GetBiome(int biomeID) {
    return biomes(i);
}

BiomeList::SetBiome(int biomeID, char biomeColor) {
    biomes[biomeID] = biomeColor;
}

BiomeList::GetLength(int biomeID) {
    return biomes.size();
}

