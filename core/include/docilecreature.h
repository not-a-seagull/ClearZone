#include <iostream>
using namespace std;
#ifndef DOCILECREATURE_H
#define DOCILECREATURE_H
class DocileCreature : public Entity {
    protected:
        int aggression;
    public:
        DocileCreature();
        DocileCreature(int, int, long, int, int, int);

        void takeAction();
};
#endif