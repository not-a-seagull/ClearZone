#include <iostream>
#include <vector>
using namespace std;
#ifndef CARAVAN_H
#define CARAVAN_H
#include "entity.h"
#include "item.h"
class Caravan : public Entity{
    protected:
        vector<Item> inventory;
    public:
        Caravan();
        Caravan(int, int, long, int, int, vector<Item>);

        void trade();
        void rob();
        void getRobbed();
};
#endif