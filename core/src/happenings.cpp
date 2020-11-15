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

#include <sstream>

std::function<std::string()> HappenResult::action() {
  return [this]() {
    Player *plr = this->world->get_player(); 
    std::stringstream ss;
    ss << this->text << std::endl;

    if (this->items_robbed > 0) {
      for (int i = 0; i < this->items_robbed; i++) {
        int index = std::rand() % plr->inventory.size(); 
        int id = plr->inventory[index];
        plr->inventory.erase(plr->begin() + index);
        res << "Lost " << item_id_to_name(id) << std::endl;
      } 
    }

    for (int i = 0; i < this->items.size(); i++) {
      res << "Found " << item_id_to_name(this->items[i]) << std::endl;
      plr->addItem(this->items[i]);
    }
    if (this->health_gain > 0) {
      res << "Regained " << this->health_gain << " health." << std::endl;
      plr->addHealth(this->health_gain);
    } else if (this->health_gain < 0) {
      res << "Lost " << -this->health_gain << " health." << std::endl;
      plr->addHealth(this->heatlh_gain);
    } 
  };
}

std::unique_ptr<Event> Happenings::to_event() {
  if (this->single_result) {
    return std::unique_ptr<Event>(new TextEvent((this->single_result.value().action())()));
  } else {
    Player *plr = this->world->get_player();
    std::shared_ptr<std::string[]> choices = std::make_shared<std::string[]>(this->results.size()); 
    int cursor = 0;
    std::vector<std::pair<int, int>> cursor_mapping;
    for (int i = 0; i < this->results.size(); i++) {
      auto opp = std::get<0>(this->results[i]);
      auto inv = plr->inventory;
      if (!opp.itemId || std::find(inv.begin(), inv.end(), opp.itemId.value()) != inv.end()) {
        choices[cursor] = opp.text;
        cursor_mappings.push_back(std::make_pair(cursor, i)); 
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
