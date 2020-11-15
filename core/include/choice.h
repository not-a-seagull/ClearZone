/* core/include/choice.h
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

#ifndef CZC_CHOICE_H
#define CZC_CHOICE_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

//! Event
class Event {
 public:
  virtual int get_type() const = 0;
};

#define TEXT_EVENT_TYPE 0

//! Text
class TextEvent : public Event {
 private:
  std::string text;

 public:
  TextEvent(const std::string& txt) : text(txt) {}
  const std::string& get_text() const { return text; };
  int get_type() const { return TEXT_EVENT_TYPE; }
};

#define YESNO_EVENT_TYPE 1

//! YesNo
class YesNoEvent : public Event {
 private:
  std::string prompt;
  std::function<void(bool)> result;

 public:
  YesNoEvent(const std::string& text, std::function<void(bool)> responder)
      : prompt(text), result(responder) {}
  const std::string& get_prompt() const { return this->prompt; }
  const std::function<void(bool)> get_responder() const { return this->result; }
  int get_type() const { return YESNO_EVENT_TYPE; }
};

#define CHOICE_EVENT_TYPE 2

//! Choice
class ChoiceEvent : public Event {
 private:
  std::shared_ptr<std::string[]> choices;
  ptrdiff_t num_choices;
  std::function<void(ptrdiff_t)> result;

 public:
  ChoiceEvent(std::shared_ptr<std::string[]> choicesr, ptrdiff_t num_ch,
              std::function<void(ptrdiff_t)> res)
      : choices(choicesr), num_choices(num_ch), result(res) {}
  const std::shared_ptr<std::string[]> get_choices() const {
    return this->choices;
  }
  ptrdiff_t get_num_choices() const { return this->num_choices; }
  std::function<void(ptrdiff_t)> get_responder() const { return this->result; }
  int get_type() const { return CHOICE_EVENT_TYPE; }
};

#define QUIT_EVENT_TYPE 3

class QuitEvent : public Event {
 public:
  QuitEvent() {}
  int get_type() const { return QUIT_EVENT_TYPE; }
};

#define MAPDPY_EVENT_TYPE 4

class MapDpyEvent : public Event {
 private:
  std::shared_ptr<char[]> data;
  int playerX;
  int playerY;
 public:
  MapDpyEvent(std::shared_ptr<char[]> dat, int x, int y) : data(dat), playerX(x), playerY(y) {}
  const std::shared_ptr<char[]> get_data() const { return this->data; }
  int get_x() const { return playerX; }
  int get_y() const { return playerY; }
  int get_type() const { return MAPDPY_EVENT_TYPE; }
};

#endif
