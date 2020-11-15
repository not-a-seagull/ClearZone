#include <iostream>
using namespace std;
#include "caravan.h"

Caravan::Caravan() : Entity() { 
    this->inventory = Item;
}

Caravan::Caravan(int a, int b, long c, int d, int e, vector<Item> f) : Entity(a, b, c, d, e) {
    this->inventory = f;
} 

void Caravan::trade();
void Caravan::rob();
void Caravan::getRobbed();

vector<Item>& getInventory() const { return this->inventory; }