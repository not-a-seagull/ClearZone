#include <iostream>
#include "entity.h"
using namespace std;

Entity::Entity() {
    this->entityType = "Blank";
    this->health = this->intelligence = this->charisma = this->dexterity = this->wisdom = this->constitution = 0;
    this->strength = 0;
    this->sexAppeal = 0; //(me)
    this->numEntities = 1;
}

Entity::Entity(string s, int heal, int intel, int charis, int dex, int wisdom, int constit, double str, long sex, int num) {
    this->entityType = s;
    this->health = heal;
    this->intelligence = intel;
    this->charisma = charis;
    this->dexterity = dex;
    this->wisdom = wisdom;
    this->constitution = constit;
    this->strength = str;
    this->sexAppeal = sex;
    this->numEntities = num;
}

string Entity::getEntityType() { return this->entityType; } 
void Entity::setEntityType(string s) { this->entityType = s; } 

int Entity::getHealth() const { return this->health; }
void Entity::setHealth(int h) { this->health = h; }

int Entity::getIntelligence() const { return this->intelligence; }
void Entity::setIntelligence(int intel) { this->intelligence = intel; }

int Entity::getCharisma() const { return this->charisma; }
void Entity::setCharisma(int c) { this->charisma = c; }

int Entity::getWisdom() const { return this->wisdom; }
void Entity::setWisdom(int w) { this->wisdom = w; }

int Entity::getConstitution() const { return this->constitution; }
void Entity::setConstitution(int c) { this->constitution = c; }

long Entity::getSexAppeal() const { return this->sexAppeal; }
void Entity::setSexAppeal(long s) { this->sexAppeal = s; }

int Entity::getNumEntities() const { return this->numEntities; }
void Entity::setNumEntities(int num) { this->numEntities = num; }

double Entity::getStrength() const { return this->strength; }
void Entity::setStrength(double s) { this->strength = s; }

void Entity::addItem(const Item& item) {
    this->inventory.push_back(item);
} 