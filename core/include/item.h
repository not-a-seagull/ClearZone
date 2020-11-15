#include <iostream>
using namespace std;
#ifndef ITEM_H
#define ITEM_H

class Item {
  protected:
    int itemID;
    int weight;
  public:
    Item();
    Item(int);

    int getItemID() const;
    void setItemID(int);
};
#endif

class Armor : public Item {
  protected:
    int armorClass;
    int speedBoost;
    string bootType, pantType, shirtType, hatType;
  public:
    Armor();
    Armor(int);
    
    void getBoot();
    void getPant();
    void getShirt();
    void getHat();

    int getSpeedBoost() const;
    int getArmorClass() const;
};

void Armor::getBoot() {
  if (this->bootType == "none") {
    this->speedBoost += 0;
  } else if (this->bootType == "leather") {
    this->speedBoost += 5;
    this->armorClass += 1;
  } else if (this->bootType == "steel toed boots") {
    this->speedBoost += 3;
    this->armorClass += 5;
  }
}

void Armor::getPant() {
  if (this->pantType == "none") {
    //do nothing for now
  } else if (this->pantType == "cloth") {
    this->armorClass += 1;
    this->speedBoost += 1;
  } else if (this->pantType == "leather") {
    this->armorClass += 5;
  } else if (this->pantType == "steel") {
    this->armorClass += 10;
    this->speedBoost -= 5;
  }
}

void Armor::getShirt() {
  if (this->shirtType == "none") {
    //do nothing for now
  } else if (this->shirtType == "cloth") {
    this->armorClass += 3;
  } else if (this->shirtType == "leather") {
    this->armorClass += 7;
  } else if (this->shirtType == "steel") {
    this->armorClass += 15;
    this->speedBoost -= 5;
  }
}

void Armor::getHat() {
  if (this->hatType == "none") {
    //do nothing for now
  } else if (this->hatType == "beanie") {
    //do nothing for now
  } else if (this->hatType == "duckbill") {
    //do nothing for now
  } else if (this->hatType == "helmet") {
    this->armorClass += 5;
    this->speedBoost -= 1;
  }
}

int Armor::getSpeedBoost() const { return this->speedBoost; }
int Armor::getArmorClass() const { return this->armorClass; }


class Weapon : public Item {
  protected:
    int damageModifyer;
    int range; //range of 1 is melee
  public:
    Weapon();
    Weapon(int);
};

