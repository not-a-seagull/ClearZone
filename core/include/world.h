#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "biome.h"
#include "cell.h"
#include "choice.h"
#include "player.h"

using namespace std;

class World {
 protected:
  Biome biomeList;
  unique_ptr<unique_ptr<Cell[]>[]> Cells;
  int width;
  int height;

  std::queue<std::unique_ptr<Event>> event_queue;
  std::vector<std::unique_ptr<Entity>> entities;

  void generate_events();
  std::shared_ptr<char[]> compile_map();
 public:
  World();

  void moveEntity(string);
  unique_ptr<Event> next_event();
  Player *get_player();
};
#endif
