#ifndef BIOME_H
#define BIOME_H

#include <iostream>

using namespace std;


// class Biome {
//     protected:
//         string color;
//         int ID;
//     public:
//         Biome();
//         Biome(string, int);

//         string getColor() const;
//         void setColor(string);

//         int getID() const;
//         void setID(int);
// };

class BiomeList {
    protected:
        std::vector(char) biomes = {'%', '\'', '\"', '{'};

    public: 
        void addBiome(char biomeColor);

        char getBiome(int biomeID);
        void setBiome(int biomeID, char biomeColor);

        int getLength();
}
#endif
