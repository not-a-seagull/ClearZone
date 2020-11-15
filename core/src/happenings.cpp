/* core/include/happenings.h
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

#include "happenings.h"
#include "player.h"
#include "world.h"

#include <sstream>

std::function<std::string()> HappenResult::action() {
  return [this]() {
    Player *plr = this->world->get_player(); 
    std::stringstream res;
    res << this->text << std::endl;

    if (this->items_robbed > 0) {
      for (int i = 0; i < this->items_robbed; i++) {
        int index = std::rand() % plr->inventory.size(); 
        int id = plr->inventory[index].getItemID();
        plr->inventory.erase(plr->inventory.begin() + index);
        res << "Lost " << item_id_to_name(id) << std::endl;
      } 
    }

    for (int i = 0; i < this->items.size(); i++) {
      res << "Found " << item_id_to_name(this->items[i].getItemID()) << std::endl;
      plr->addItem(this->items[i]);
    }
    if (this->health_gain > 0) {
      res << "Regained " << this->health_gain << " health." << std::endl;
      plr->setHealth(plr->getHealth() + this->health_gain);
    } else if (this->health_gain < 0) {
      res << "Lost " << -this->health_gain << " health." << std::endl;
      plr->setHealth(plr->getHealth() + this->health_gain);
    } 

    return res.str();
  };
}

std::unique_ptr<Event> Happenings::to_event() {
  this->world->push_event(std::unique_ptr<Event>(new TextEvent(this->prompt)));

  if (this->single_result) {
    return std::unique_ptr<Event>(new TextEvent((this->single_result.value().action())()));
  } else {
    Player *plr = this->world->get_player();
    std::shared_ptr<std::string[]> choices = std::shared_ptr<std::string[]>(new std::string[this->results.size()]); 
    int cursor = 0;
    std::vector<std::pair<int, int>> cursor_mapping;
    for (int i = 0; i < this->results.size(); i++) {
      auto opp = std::get<0>(this->results[i]);
      auto inv = plr->inventory;
      if (!opp.itemId || std::find(inv.begin(), inv.end(), opp.itemId.value()) != inv.end()) {
        choices[cursor] = opp.text;
        cursor_mapping.push_back(std::make_pair(cursor, i)); 
        cursor++;
      }
    }

    return std::unique_ptr<Event>(new ChoiceEvent(choices, cursor_mapping.size(), [cursor_mapping, this](ptrdiff_t select) {
      int real;
      for (int i = 0; i < cursor_mapping.size(); i++) {
        if (std::get<0>(cursor_mapping[i]) == select) {
          real = std::get<1>(cursor_mapping[i]);
        }
      }

      this->world->push_event(std::unique_ptr<Event>(new TextEvent((std::get<1>(this->results[real]).action())())));
    }));
  }
}

std::shared_ptr<std::shared_ptr<Happenings[]>[]> get_happenings(World *world) {
  std::shared_ptr<std::shared_ptr<Happenings[]>[]> res = std::shared_ptr<std::shared_ptr<Happenings[]>[]>(new std::shared_ptr<Happenings[]>[6]);
  res[0] = std::shared_ptr<Happenings[]>(new Happenings[HAPPENINGS_PER_BIOME]);

  res[0][0] = Happenings(world, "You encounter a lone hut in the woods. Smoke billows from an opening in the top.");
  HappenResult hr1(world, "The hut has long since been abandoned. You find a knife for your troubles.");
  HappenResult hr2(world, "You move on.");
  HappenResult hr3(world, "Your extra caution was unneeded. The hut is abandoned, and you find nothing useful.");
  Opportunity o1("Enter the hut.");
  Opportunity o2("Don't take your chances.");
  Opportunity o3("Walk in brandishing a knife.", 2);
  res[0][0].results.push_back(std::make_pair(o1, hr1));
  res[0][0].results.push_back(std::make_pair(o2, hr2));
  res[0][0].results.push_back(std::make_pair(o3, hr3));

  res[0][1] = Happenings(world, "You find the decaying carcass of a wolf.", HappenResult(world, "The meat is unappetizing."));

//  res[0][1] = res[0][0];

//  HappenResult hr4(world, "A wolf leaps out and attacks you!", -1);
//  HappenResult hr4(world, "Your caution is rewarded. You stab a wolf as it leaves the hut.");
//  res[0][1].results[0] = std::make_pair;
//  res[0][1]

  res[1] = std::shared_ptr<Happenings[]>(new Happenings[HAPPENINGS_PER_BIOME]);

  res[1][0] = Happenings(world, "The desert sprawls far and wide for as far as the eye can see.", HappenResult(world, "You feel nothing but dehydration."));
  res[1][1] = Happenings(world, "Birds circle above you.", HappenResult(world, "It doesn't do much for confidence"));
  
  res[2] = std::shared_ptr<Happenings[]>(new Happenings[HAPPENINGS_PER_BIOME]);

  res[2][0] = Happenings(world, "The peace inspired by the plains is a serene break from the chaos of life.", HappenResult(world, "You feel releaxed."));
  res[2][1] = Happenings(world, "An eerie screeching noise upsets the quiet.", HappenResult(world, "You can't tell where it came from..."));

  res[3] = std::shared_ptr<Happenings[]>(new Happenings[HAPPENINGS_PER_BIOME]);

  res[3][0] = Happenings(world, "The jungle may be hard to naviate, but at least you're safe from predators, for now.", HappenResult(world, "You should get out of here as soon as possible."));
  res[3][1] = Happenings(world, "You see something swinging from the trees.", HappenResult(world, "At least, you thought you saw something."));

  res[4] = std::shared_ptr<Happenings[]>(new Happenings[HAPPENINGS_PER_BIOME]);

  res[4][0] = Happenings(world, "The city falls down around you. What could have possible happened here?", HappenResult(world, "You feel nervous."));
  res[4][1] = Happenings(world, "You hear yelling, but can't tell where it comes from.", HappenResult(world, "What does it all mean?"));

  res[5] = std::shared_ptr<Happenings[]>(new Happenings[HAPPENINGS_PER_BIOME]);

  res[5][0] = Happenings(world, "", )
  return res;
}
