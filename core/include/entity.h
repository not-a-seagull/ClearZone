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
  int subclass;

  int health;
  long sexAppeal;

  vector<Item> inventory;

 public:
  int indexX;
  int indexY;

  Entity();
  Entity(string s, int heal, int intel, int charis, int dex, int wisdom,
         int constit, double str, long sex, int num);

  int getSubclass() const { return this->subclass; }
  int getEntityType() const;
  void setEntityType(int);

  int getNumEntities() const;
  void setNumEntities(int);

  long getSexAppeal() const;
  void setSexAppeal(long);

  void addItem(Item);
  void moveEntity(int dir);
};
#endif
