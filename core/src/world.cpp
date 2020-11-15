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

  std::unique_ptr<Event> mde = std::unique_ptr<Event>(new MapDpyEvent(tiles));
  std::unique_ptr<Event> ce = std::unique_ptr<Event>(
      new ChoiceEvent(get_directionals(), 5, [](ptrdiff_t choice) {
        // TODO
      }));
  std::unique_ptr<Event> te =
      std::unique_ptr<Event>(new TextEvent("Choose where to go"));

  this->event_queue.push(std::move(mde));
  this->event_queue.push(std::move(te));
  this->event_queue.push(std::move(ce));
}
