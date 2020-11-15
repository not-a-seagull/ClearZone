#include "entity.h"
#include <iostream>
using namespace std;
Entity::Entity() {
    this->entityType = 0;
    this->sexAppeal = 0; //(me)
}


Entity::Entity(int s, int heal, long sex, int inx, int iny) {
  this->entityType = s;
  this->health = heal;
  this->sexAppeal = sex;
  this->indexX = inx;
  this->indexY = iny;
}

int Entity::getEntityType() const { return this->entityType; }
void Entity::setEntityType(int s) { this->entityType = s; }

int Entity::getSpeed() const { return this->speed; }
void Entity::setSpeed(int s) { this->speed = s; }

int Entity::getHealth() const { return this->health; }
void Entity::setHealth(int h) { this->health = h; }

void Entity::addItem(const Item& item) { this->inventory.push_back(item); }

void Entity::moveEntity(int dir) {
  if (dir == UP) {
    this->indexY -= 1;
  } else if (dir == DOWN) {
    this->indexY += 1;
  } else if (dir == LEFT) {
    this->indexX -= 1;
  } else if (dir == RIGHT) {
    this->indexX += 1;
  }
}
