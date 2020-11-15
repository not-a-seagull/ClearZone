#include <iostream>
using namespace std;
#ifndef HUMANOID_H
#define HUMANOID_H
#include "entity.h"
class Humanoid : public Entity {
 protected:
  int constitution, intelligence, wisdom, charisma, dexterity, strength;
  double hunger;
  int damage;

 public:
  Humanoid();
  Humanoid(int, int, long, int, int, int,int, int, int, int, int);

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

  int getStrength() const;
  void setStrength(int);

  int getHunger() const;
  void setHunger(int);
  
  int getDamage() const;
  void updateDamage(int damage);

  long getSexAppeal() const;
  void setSexAppeal(long);
};
#endif
