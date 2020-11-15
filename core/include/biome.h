#ifndef BIOME_H
#define BIOME_H

#include <iostream>

using namespace std;


class Biome {
    protected:
        string color;
        int ID;
    public:
        Biome();
        Biome(string, int);

        string getColor() const;
        void setColor(string);

        int getID() const;
        void setID(int);
};
#endif
