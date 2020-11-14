#include <iostream>

using namespace std;
#ifndef BIOME_H
#define BIOME_H
class Biome {
    protected:
        string color;
        int ID;
    public:
        Biome();
        Biome(string, ID);

        string getColor() const;
        void setColor(string);

        int getID() const;
        void setID(int);
};
#endif