#include <iostream>
using namespace std;
#ifndef HUMANOID_H
#define HUMANOID_H
#include "entity.h"
class Humanoid : public Entity {
    protected: 
        int constitution, intelligence, wisdom, charisma, dexterity, strength;
    public:
        Humanoid();
        
        int getDexterity() const;
        void setDexterity(int);

        int getCharisma() const;
        void setCharisma(int);

        int getWisdom() const;
        void setWisdom(int);

        int getIntelligence() const;
        void setIntelligence(int);

        int getConstitution() const;
        void setConstitution(int);  

        double getStrength() const;
        void setStrength(double);
};
#endif