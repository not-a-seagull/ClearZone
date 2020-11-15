#include <iostream>
#include <vector>

using namespace std;

#ifndef ENTITY_H
#define ENTITY_H

class Entity {
 protected:
  int entityType;

  int health;
  long sexAppeal;

  vector<Item> inventory;

 public:
  Entity();
  Entity(int, int, long, int, int);

  int getEntityType() const;
  void setEntityType(int);
  string getEntityType() const;
  void setEntityType(string);

  int getNumEntities() const;
  void setNumEntities(int);

  long getSexAppeal() const;
  void setSexAppeal(long);

  void addItem(Item);
};
#endif
