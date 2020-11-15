#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <memory>
#include <queue>

#include "cell.h"
#include "choice.h"

using namespace std;

class World {
 protected:
  BiomeList biomeList;
  unique_ptr<unique_ptr<Cell[]>[]> Cells;

  std::queue<std::unique_ptr<Event>> event_queue;

  void generate_events();

 public:
  World();

  void moveEntity(string);
  unique_ptr<Event> next_event();
};
#endif
