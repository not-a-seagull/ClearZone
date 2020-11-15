#include <iostream>
using namespace std;
#ifndef ITEM_H
#define ITEM_H
struct Item {
    protected:
        int itemID;
    public:
        Item();
        Item(int);

        int getItemID() const;
        void setItemID(string);
};
#endif