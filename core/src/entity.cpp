#include <iostream>
#include "entity.h"
using namespace std;

Entity::Entity() {
    this->entityType = 0;
    this->sexAppeal = 0; //(me)
    this->numEntities = 1;
}

Entity::Entity(int s, int heal, long sex, int inx, int iny) {
    this->entityType = s;
    this->health = heal;
    this->sexAppeal = sex;
    this->indexX = inx;
    this->indexY = iny;
}

int Entity::getEntityType() { return this->entityType; } 
void Entity::setEntityType(int s) { this->entityType = s; } 

int Entity::getHealth() const { return this->health; }
void Entity::setHealth(int h) { this->health = h; }



void Entity::addItem(const Item& item) {
    this->inventory.push_back(item);
} 