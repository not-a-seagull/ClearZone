#include "biome.h"

#include <iostream>
using namespace std;

Biome::AddBiome(char biomeColor) { biomes.push_back(biomeColor); }

Biome::GetBiome(int biomeID) { return biomes(i); }

Biome::SetBiome(int biomeID, char biomeColor) { biomes[biomeID] = biomeColor; }

Biome::GetLength(int biomeID) { return biomes.size(); }
