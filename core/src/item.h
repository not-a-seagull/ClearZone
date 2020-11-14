#include <iostream>
using namespace std;
#ifndef ITEM_H
#define ITEM_H
class Item {
    protected:
        int itemID;
    public:
        Item();
        Item(int);

        int getItemID() const;
        void setItemID(string);
};
#endif