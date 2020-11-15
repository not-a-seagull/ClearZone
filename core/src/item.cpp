#include <iostream>
using namespace std;
#include "item.h"

Item::Item(int[10] items) {
    for (int i = 0; i < 10; i++) {
        this->itemIDs[i] = items[i];
    }
}

int Item::getItemID(int index) const { return this->itemIDs[index]; }


