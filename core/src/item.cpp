#include <iostream>
using namespace std;
#include "item.h"

Item::Item() { this->itemID = 0; }

Item::Item(int i) { this->itemID = i; }

int Item::getItemID() const {
  return this->itemID;
}
void Item::setItemID(int i) { this->itemID = i; }
