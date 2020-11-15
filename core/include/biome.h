#ifndef BIOME_H
#define BIOME_H

#include <iostream>

using namespace std;

class Biome {
 protected:
  static std::vector<char> biomes = {'%', '\'', '\"', '{'};

 public:
  static void addBiome(char biomeColor);

  static char getBiome(int biomeID);
  static void setBiome(int biomeID, char biomeColor);

  static int getLength();
};
#endif
