#include "entity.h"

#include <iostream>
using namespace std;

Entity::Entity(string s, int heal, int intel, int charis, int dex, int wisdom,
               int constit, double str, long sex, int num) {
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

void Entity::addItem(const Item& item) { this->inventory.push_back(item); }

void Entity::moveEntity(string dir) {
  this->indexX;
  this->indexY;
  if (dir == "UP") {
    this->
  } else if (dir == "DOWN") {
  } else if (dir == "LEFT") {
  } else if (dir == "RIGHT") {
  }
}
