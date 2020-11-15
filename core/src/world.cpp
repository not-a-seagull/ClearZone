/* core/src/world.cpp
 *
 * Clear Zone is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Clear Zone is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Clear Zone.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "world.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>

#include "choice.h"

static std::shared_ptr<std::string[]> directionals;

std::shared_ptr<std::string[]> get_directionals() {
  if (!directionals) {
    directionals = std::shared_ptr<std::string[]>(new std::string[5]);
    directionals[0] = "Up";
    directionals[1] = "Left";
    directionals[2] = "Right";
    directionals[3] = "Down";
    directionals[4] = "Stay Still";
  }

  return directionals;
}

// I'm very tired
World::World() { 
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      //            Cells[i][j] = Cell;
    }
  }
}

std::unique_ptr<Event> World::next_event() {
  while (this->event_queue.empty()) {
    this->generate_events();
  }
  std::unique_ptr<Event> ev = std::move(this->event_queue.front());
  this->event_queue.pop();
  return ev;
}

void World::generate_events() {
  // just generate move and render events for now
  std::shared_ptr<char[]> tiles = std::shared_ptr<char[]>(new char[32 * 32]);
  memset(tiles.get(), 'B', 32 * 32);

  Player *ply = this->get_player();
  int x = ply->indexX;
  int y = ply->indexY;

  std::unique_ptr<Event> mde = std::unique_ptr<Event>(new MapDpyEvent(tiles, x, y));
  std::unique_ptr<Event> ce = std::unique_ptr<Event>(
      new ChoiceEvent(get_directionals(), 5, [this](ptrdiff_t choice) {
        Player *ply = this->get_player();
        switch (choice) {
          case 0: ply->moveEntity(UP); break;
          case 1: ply->moveEntity(LEFT); break;
          case 2: ply->moveEntity(RIGHT); break;
          case 3: ply->moveEntity(DOWN); break;
        } 
      }));
  std::unique_ptr<Event> te =
      std::unique_ptr<Event>(new TextEvent("Choose where to go"));

  this->event_queue.push(std::move(mde));
  this->event_queue.push(std::move(te));
  this->event_queue.push(std::move(ce));
}

Player *World::get_player() {
  for (ptrdiff_t i = 0; i < this->entities.size(); i++) {
    if (this->entities[i]->getSubclass() == PLAYER_SUBCLASS) {
      return static_cast<Player *>(this->entities[i].get());
    }
  }
  return nullptr;
}

std::shared_ptr<char[]> World::compile_map() {
  Player *ply = this->get_player();
  int left = ply->indexX - 16;
  int top = ply->indexY - 16;
  if (left < 0) left = 0;
  if (top < 0) top = 0;

  int right = left + 32;
  int bottom = top + 32;
  
  if (right > this->width) {
    right = this->width;
    left = right - 32;
  }
  if (bottom > this->height) {
    bottom = this->height;
    top = bottom - 32;
  }

  std_shared<char[]> res = std::make_shared<char[]>(32*32);
  for (int i = left; i < right; i++) {
    for (int j = top; j < bottom; j++) {
      res[(j * 32) + i] = Biome::GetBiome(this->Cells[j][i]);
    }
  }

  return res;
}
