#include <iostream>
#include "humanoid.h"
using namespace std;

Humanoid::Humanoid() : Entity() {
    this->strength = 10;
    this->dexterity = 10;
    this->constitution = 10;
    this->intelligence = 10;
    this->wisdom = 10;
    this->charisma = 10;
}

Humanoid::Humanoid(int s, int heal, long sex, int inx, int iny, int strength, int dex, int constitution, int intelligence, int wisdom, int charisma) : Entity(s, heal, sex, inx, iny) {
    this->strength = strength;
    this->dexterity = dex;
    this->constitution = constitution;
    this->intelligence = intelligence;
    this->wisdom = wisdom;
    this->charisma = charisma;
}

int Humanoid::getIntelligence() const { return this->intelligence; }
void Humanoid::setIntelligence(int intel) { this->intelligence = intel; }

int Humanoid::getCharisma() const { return this->charisma; }
void Humanoid::setCharisma(int c) { this->charisma = c; }

int Humanoid::getWisdom() const { return this->wisdom; }
void Humanoid::setWisdom(int w) { this->wisdom = w; }

int Humanoid::getConstitution() const { return this->constitution; }
void Humanoid::setConstitution(int c) { this->constitution = c; }

long Humanoid::getSexAppeal() const { return this->sexAppeal; }
void Humanoid::setSexAppeal(long s) { this->sexAppeal = s; }

int Humanoid::getNumEntities() const { return this->numEntities; }
void Humanoid::setNumEntities(int num) { this->numEntities = num; }

int Humanoid::getStrength() const { return this->strength; }
void Humanoid::setStrength(int s) { this->strength = s; }