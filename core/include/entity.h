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
<<<<<<< HEAD
    protected:
        int entityType;
        int health;
        long sexAppeal;
        int indexX, indexY;
    public:
        Entity();
        Entity(int, int, long, int, int);
=======
 protected:
  int entityType;
  int subclass;
>>>>>>> 7018c1d6920174aba3d15f49a9489bcb59fe70de

        int getEntityType() const;
        void setEntityType(int);

        int getNumEntities() const;
        void setNumEntities(int);

<<<<<<< HEAD
        int getHealth() const;
        void setHealth(int);

        long getSexAppeal() const;
        void setSexAppeal(long);

        void addItem(Item);
=======
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
>>>>>>> 7018c1d6920174aba3d15f49a9489bcb59fe70de
};
#endif
