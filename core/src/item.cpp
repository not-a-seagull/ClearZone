#include <iostream>
using namespace std;
#include "item.h"

Item::Item(int id) {
  this->itemID = id;
}

int Item::getItemID() const { return this->itemID; }

std::string item_id_to_name(int id) {
  switch (id) {
    case 1: return "fists";
    case 2: return "knife";
    case 3: return "spear";
    case 4: return "katana";
    case 5: return "longbow";
    case 6: return "crossbow";
    case 7: return "rifle";
    case 8: return "leather shoes";
    case 9: return "steel toed shoes";
    case 10: return "pants";
    case 11: return "leather pants";
    case 12: return "steel pants";
    case 13: return "shirt";
    case 14: return "leather tunic";
    case 15: return "steel breastplate";
    case 16: return "helmet";
    default: return "none";
  }
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

Armor::Armor() {
  this->bootType = this->pantType = this->shirtType = this->hatType = "none";
}

Armor::Armor(int ty) {
  int bootType, pantType, shirtType, hatType;
  bootType = pantType = shirtType = hatType = ty;
  this->bootType = (bootType == 0)?"none":((bootType == 1)?"leather":(bootType == 2)?"steel toed boots":"none");
  this->pantType = (pantType == 0)?"none":((pantType == 1)?"cloth":((pantType == 2)?"leather":(pantType == 3)?"steel":"none"));
  this->shirtType = (shirtType == 0)?"none":((shirtType == 1)?"cloth":((shirtType == 2)?"leather":(shirtType == 3)?"steel":"none"));
  this->hatType = (hatType == 0)?"none":((hatType == 1)?"beanie":((hatType == 2)?"duckbill":(hatType == 3)?"helmet":"none"));
}

int Armor::getSpeedBoost() const { return this->speedBoost; }
int Armor::getArmorClass() const { return this->armorClass; }


Weapon::Weapon() : Item(1) {

}

Weapon::Weapon(int meleeType) {
  this->itemID = meleeType;
  this->damageModifyerM = meleeType + 1;
  this->damageR = meleeType * 15;

}

void Weapon::updateDialougue() {
  this->weaponDialougue = (this->itemID == 1)?"punch enemy" : (this->itemID == 2 || this->itemID == 3 || this->itemID == 4)?"stab enemy":"shoot enemy";
}
int Weapon::getArrows() const { return this->arrows; }
int Weapon::getBullets() const { return this->bullets; }

string Weapon::getRanged() const {
  return this->bowType;
}
string Weapon::getMelee() const { return this->meleeType; }

