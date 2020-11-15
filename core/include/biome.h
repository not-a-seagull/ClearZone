#ifndef BIOME_H
#define BIOME_H

#include <iostream>

using namespace std;

class Biome {
 protected:
  std::vector<char> biomes = {'%', '\'', '\"', '{'};

 public:
  void addBiome(char biomeColor);

  char getBiome(int biomeID);
  void setBiome(int biomeID, char biomeColor);

  int getLength();
};
#endif
