/* core/src/player.cpp
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

#include "player.h"

Player::Player() { this->subclass = PLAYER_SUBCLASS; }

Player::Player(int a, int b, long c, int d, int e) 
    : Entity(a, b, c, d, e) {
  this->subclass = PLAYER_SUBCLASS;
}
