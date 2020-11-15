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

std::string item_id_to_name(int id); 

class Item {
  protected:
    int itemID;
  public:
    Item() : itemID(0) {}
    Item(int); //items

    int getItemID() const;
    bool operator ==(const Item& other) { return other.itemID == this->itemID; }
};

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

class Weapon : public Item {
  protected:
    int damageModifyerM, damageR;
    int arrows, bullets;
    string bowType, meleeType; //will display bowtype and meleetype as choices in combat
  public:
    Weapon();
    Weapon(int);

    int getArrows() const;
    string getRanged() const;
    string getMelee() const;
    int getBullets() const;
};

#endif
