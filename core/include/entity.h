#include <iostream>
#include <vector>

using namespace std;

#ifndef ENTITY_H
#define ENTITY_H

#include "item.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

class Entity {
    protected:
        int entityType;
        int health;
        long sexAppeal;
        int speed;
    public:
        vector<Item> inventory;
        Entity();
        Entity(int, int, long, int, int);

  int subclass;
  int indexX;
  int indexY;

  int getSubclass() const { return this->subclass; }
  int getEntityType() const;
  void setEntityType(int);

  long getSexAppeal() const;
  void setSexAppeal(long);

  int getSpeed() const;
  void setSpeed(int s);

  int getHealth() const;
  void setHealth(int h);

  void addItem(const Item&);
  void moveEntity(int dir);
};
#endif
