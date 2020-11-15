/* gtkui/include/ui.h
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

#ifndef CZG_UI_H
#define CZG_UI_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <functional>
#include <memory>
#include <optional>

#include "animation.h"
#include "world.h"

class GInterface;

class ChoiceResult {
 public:
  GInterface *v;
  ptrdiff_t choice;
  ChoiceResult(GInterface *vc, ptrdiff_t choicec) : v(vc), choice(choicec) {}
};

class GInterface {
 public:
  //! Magic byte to match off of to verify this is a GInterface.
  uint32_t magic;
  //! Is this interface activated yet?
  bool activated;

  GtkApplication *app;
  //! Parent window of everything.
  GtkWidget *parent;
  //! Container for text.
  GtkWidget *textbox;
  //! Container for drawings.
  GtkWidget *drawbox;
  //! Container for current choicebox.
  GtkWidget *choicebox;

  //! Current image surface for cairo.
  cairo_surface_t *current_drawbox;
  //! If we are running an animation, this will be non-null.
  Animation *current_animation;
  //! Current frame of the animation.
  ptrdiff_t anim_frame;

  //! Allocated choice objects.
  std::vector<ChoiceResult *> choices;

  //! Pointer to the world object.
  std::unique_ptr<World> world;

  std::optional<std::function<void(bool)>> yesno_responder;
  std::optional<std::function<void(ptrdiff_t)>> choice_responder;

  int argc;
  char **argv;

 public:
  GInterface(int argc, char **argv);
  ~GInterface();

  int run();
  void stop();
  void initialize_world();
  void world_tick();

  //! Display some text on the GInterface.
  void dpy_text(const char *line);

  //! Display a choice on the GInterface, and add a callback for when the choice
  //! is selected.
  void dpy_choice(std::shared_ptr<std::string[]> choices, ptrdiff_t num_choices,
                  std::function<void(ptrdiff_t)>);

  //! Display a yes/no question on the GInterface, and add a callback for when
  //! the choice is selected.
  void dpy_yesno(const char *prompt, std::function<void(bool)>);

  //! Display an enemy on the GInterface.
  void dpy_enemy(const unsigned char *img, uint32_t width, uint32_t height);

  //! Run an animation on the GInterface.
  void animation_overlay(const Animation *anim);

  //! Display a map on the GInterface.
  void dpy_map(std::shared_ptr<char[]> img);
};

#endif  // CZG_UI_H
