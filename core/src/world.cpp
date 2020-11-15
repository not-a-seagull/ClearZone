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
#include "PerlinNoise.hpp"
#include "humanoid.h"

static char BIOME_TABLE[] = {
   'F', 'D', 'P', 'J', 'A', 'W'
};
static int BIOME_COUNT = 6;

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
void World::doCombat(Humanoid& player, Entity& enemy) {
  bool playerTurn = true;
  while (player.getHealth() > 0 && enemy.getHealth() > 0) {
    std::shared_ptr<std::string[]> weapons = std::shared_ptr<std::string[]>(new std::string[2]);
    weapons[0] = "Fists";
    weapons[1] = "Knife";

    this->push_event(std::unique_ptr<Event>(new ChoiceEvent(weapons, 2, [this, player, playerTurn, enemy](ptrdiff_t r) mutable {
    int playerWeaponChoiceID = r + 1;;
    int playerDamageMultiplyer = 1;
    //display weapon dialougue this->weaponDialougue to user
    switch(playerWeaponChoiceID) {
      case 1: playerDamageMultiplyer = 1;
      case 2: playerDamageMultiplyer = 2;
      case 3: playerDamageMultiplyer = 3;
      case 4: playerDamageMultiplyer = 4;
      case 5: playerDamageMultiplyer = 1;
      case 6: playerDamageMultiplyer = 3;
      case 7: playerDamageMultiplyer = 5;
    }
    if (player.getSpeed() >= enemy.getSpeed()) {
      
      if (playerTurn) {
        enemy.setHealth(enemy.getHealth() - (player.getStrength() * playerDamageMultipler));
        playerTurn = false;
      } else if (!playerTurn) {
        player.setHealth(player.getHealth() - 1);
        playerTurn = true;
      }
    } else if (player.getSpeed() < enemy.getSpeed()) {
      if (!playerTurn) {
        player.setHealth(player.getHealth() - 1);
        playerTurn = true;
      } else if (playerTurn) {
        enemy.setHealth(enemy.getHealth() - (player.getStrength() * playerDamageMultipler));
        playerTurn = false;
      }
    }
    })));
  }
}
// I'm very tired
World::World(int width, int height, int octaves, int seed, float scale) { 
//  siv::PerlinNoise perlin(seed);
  std::srand(time(NULL));
  const siv::PerlinNoise perlin(seed);
  std::unique_ptr<Entity> plr = std::unique_ptr<Entity>(new Player(0, 0, 0, 0, 0));
  plr->indexX = (int)width / 2;
  plr->indexY = (int)width / 2;
  this->width = width;
  this->height = height;
  this->entities.push_back(std::move(plr));
  this->cells = std::move(std::make_unique<std::unique_ptr<Cell[]>[]>(width));
  for (int i = 0; i < width; i++) {
    this->cells[i] = std::move(std::make_unique<Cell[]>(height));
    for (int j = 0; j < height; j++) {
      float val = (float)ValueNoise2D_2D((double) i / scale,(double) j / scale);
      //float val = (float)std::rand() / (float)RAND_MAX;
      this->cells[i][j] = Cell((int) (val * BIOME_COUNT));
    }
  }

  std::cout << "dfjslk" << std::endl;

  // This loop iterates over the map and determines river tiles.
  for (int i = 0; i < width; i++) {
    this->cells[i] = std::move(std::make_unique<Cell[]>(height));
    for (int j = 0; j < height; j++) {
      float riverProbability = 0.1f; // Probability that a river / stream tile will on this cell;
      for(int x = -1; x < 1; x++) {
        for(int y = -1; y < 1; y++) {
          if(this->cells[max(i + x, 0)][max(j + y, 0)].GetBiome() == 5) {
              riverProbability += 0.1f;
          }
        }
      }
      if((float)std::rand() / RAND_MAX < riverProbability) {
        this->cells[i][j] = Cell(5);
      }
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
  int x,y;
  std::shared_ptr<char[]> tiles = this->compile_map(x, y); 

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

        // get an event from the new tile
        Happenings *hs = this->happenings[this->cells[ply->indexX][ply->indexY].GetBiome()].get();
        this->push_event(hs[std::rand() % HAPPENINGS_PER_BIOME].to_event());
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

std::shared_ptr<char[]> World::compile_map(int &playerx, int &playery) {
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

  playerx = ply->indexX - left;
  playery = ply->indexY - top;

  std::shared_ptr<char[]> res = std::shared_ptr<char[]>(new char[32*32]); 
  memset(res.get(), 'B', 32 * 32);
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; j++) {
      res[(j * 32) + i] = BIOME_TABLE[this->cells[left + i][top + j].GetBiome()];
    }
  }

  return res;
}

void World::push_event(std::unique_ptr<Event> evt) {
  this->event_queue.push(std::move(evt));
}
