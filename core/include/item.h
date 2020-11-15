#include <iostream>
using namespace std;
#ifndef ITEM_H
#define ITEM_H

//0 - none
//1 - fists
//2 - knife
//3 - spear
//4 - katana
//5 - longbow
//6 - crossbow
//7 - rifle
//8 - leather shoes
//9 - steel toed shoes
//10 - pants
//11 - leather pants
//12 - steel pants
//13 - shirt
//14 - leather tunic
//15 - steel breastplate
//16 - helmet
class Item {
  protected:
    int[10] itemIDs;
  public:
    Item(int[10]); //items

    int getItemID(int) const;
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

Armor::Armor() {
  this->bootType = this->pantType = this->shirtType = this->hatType = "none";
}

Armor::Armor(int bootType, int pantType, int shirtType, int hatType) {
  this->bootType = (bootType == 0)?"none":((bootType == 1)?"leather":(bootType == 2)?"steel toed boots":"none");
  this->pantType = (pantType == 0)?"none":((pantType == 1)?"cloth":((pantType == 2)?"leather":(pantType == 3)?"steel":"none"));
  this->shirtType = (shirtType == 0)?"none":((shirtType == 1)?"cloth":((shirtType == 2)?"leather":(shirtType == 3)?"steel":"none"));
  this->hatType = (hatType == 0)?"none":((hatType == 1)?"beanie":((hatType == 2)?"duckbill":(hatType == 3)?"helmet":"none"));
}
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
    int damageModifyerM, damageR;
    int arrows, bullets;
    string bowType, meleeType; //will display bowtype and meleetype as choices in combat
  public:
    Weapon();
    Weapon(int, int);

    int getArrows() const;
    int getBullets() const;
};

Weapon::Weapon() {
  this->itemIDs[0] = 1;
  this->itemIDs[1] = 0;
}

Weapon::Weapon(int meleeType, int bowType) {
  this->itemIDs[0] = meleeType;
  this->damageModifyerM = meleeType + 1;
  this->itemIDs[1] = bowType;
  this->damageR = bowType * 15;

}

int Weapon::getArrows() const { return this->arrows; }
int Weapon::getBullets() const { return this->bullets; }

string Weapon::getRanged() const {
  if (this->itemIDs[]
  return this->bowType; 
}
string Weapon::getMelee() const { return this->meleeType; }