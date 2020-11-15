#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "cell.h"
#include "choice.h"
#include "happenings.h"
#include "player.h"
#include "humanoid.h"
//#include "PerlinNoise.hpp"

using namespace std;

class World {
 protected:
  unique_ptr<unique_ptr<Cell[]>[]> cells;
  int width;
  int height;

  std::queue<std::unique_ptr<Event>> event_queue;
  std::vector<std::unique_ptr<Entity>> entities;
  std::shared_ptr<std::shared_ptr<Happenings[]>[]> happenings;

  void generate_events();
  std::shared_ptr<char[]> compile_map(int &px, int &py);
 public:
  World(int width, int height, int octaves, int seed, float scale);

  unique_ptr<Event> next_event();
  Player *get_player();
  void push_event(std::unique_ptr<Event>);
};
#endif
