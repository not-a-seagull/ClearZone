#include <iostream>
#include <vector>

using namespace std;

#ifndef ENTITY_H
#define ENTITY_H
#include "item.h"
class Entity {
    protected:
        string entityType;
        int health, constitution, intelligence, wisdom, charisma, dexterity;
        double strength;
        long sexAppeal;
        int numEntities;
        vector<Item> inventory;
    public:
        Entity();
        Entity(string, int, int, int, int, int, int, double, long, int);

        string getEntityType() const;
        void setEntityType(string);

        int getNumEntities() const;
        void setNumEntities(int);

        int getHealth() const;
        void setHealth(int);

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

        long getSexAppeal() const;
        void setSexAppeal(long);

        void addItem(Item);
};
#endif