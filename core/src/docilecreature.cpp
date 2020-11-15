#include "docilecreature.h"
#include <iostream>
using namespace std;

DocileCreature::DocileCreature() : Entity() {
    this->aggression = 0;
}

DocileCreature::DocileCreature(int a, int b, long c, int d, int e, int f) : Entity(a, b, c, d, e) {
    this->aggression = f;  
}
void DocileCreature::takeAction() {
    if (aggression >= 5) {
        //attack 
    } else if {
        //graze
    }

}