#include <iostream>
#include <memory>
#include <queue>

#include "biome.h"
#include "cell.h"
#include "choice.h"
#include "PerlinNoise.hpp"

std:stringstream print() {
    Map map = Map(32, 32, 0.643f, 5);
    std::stringstream ss;

    for(pixel : map.pixels) {
        for(val : pixel) {
            ss << val;
        }
        ss << "\n";
    }
}

class Map {
    protected:
    int width, height;
    float frequency;

    Biome biome;

    std::vector<std::vector<char>> pixels;
    siv::PerlinNoise perlin(4243);

    public:

    Map(int width, int height, float frequency, int octaves) {
        this->width = width;
        this->height = height;
        this->octaves = octaves;

        createMap(octaves);
    }

    void createMap(int octaves) {
        for(int i = 0; i < width; i++) {
            for(int j = 0; j < width; i++) {
                float val = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);
                pixels[i][j] = biome.getBiome((int) val * Biome.biomes.size()));    
            }
        }
            
    };

}