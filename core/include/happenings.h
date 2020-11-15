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

#ifndef CZC_HAPPENINGS_H
#define CZC_HAPPENINGS_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "choice.h"
#include "item.h"

class World;

class HappenResult {
  public:
    World *world;
    std::string text;
    std::vector<Item> items;
    int health_gain; 
    std::optional<int> items_robbed;

    HappenResult(World *worldc, const std::string& textc) : world(worldc), text(textc), health_gain(0) {}
    HappenResult(World *worldc, const std::string& textc, int health_gainc) : world(worldc), text(textc), health_gain(health_gainc) {}

    HappenResult(World *worldc, const std::string& textc, int health_gainc, int items_robbedc) : world(worldc), text(textc), health_gain(health_gainc), items_robbed(items_robbedc) {}

    std::function<std::string()> action();
};

class Opportunity {
  public:
    std::string text;
    std::optional<int> itemId;

    Opportunity(const std::string& txt): text(txt) {}
    Opportunity(const std::string& txt, int itemIdc) : text(txt), itemId(itemIdc) {}
};

class Happenings {
  public: 
     World *world;
     std::string prompt;
     std::optional<HappenResult> single_result;
     std::vector<std::pair<Opportunity, HappenResult>> results;

     Happenings() {}
     Happenings(World *worldc, const std::string& promptc, HappenResult res) : world(worldc), prompt(promptc), single_result(res) {}
     Happenings(World *worldc, const std::string& promptc) : world(worldc), prompt(promptc) {}

     std::unique_ptr<Event> to_event();
};

#define HAPPENINGS_PER_BIOME 1
std::shared_ptr<std::shared_ptr<Happenings[]>[]> get_happenings(World *world);

#endif
