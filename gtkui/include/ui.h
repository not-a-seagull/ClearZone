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

#if __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "animation.h"

//! Opaque GTK object type.
typedef void GInterface;

//! Initialize the GTK system.
GInterface *ginterface_init(int argc, char **argv);

//! Drop the GTK system.
void ginterface_drop(GInterface *interface);

//! Begin running the GTK system.
int ginterface_run(GInterface *interface);

//! Display some text on the GInterface.
void ginterface_dpy_text(GInterface *interface, const char *line);

//! Display a choice on the GInterface, and add a callback for when the choice
//! is selected.
typedef void (*ChoiceSelection)(ptrdiff_t, GInterface*, void*);
void ginterface_dpy_choice(GInterface *interface, const char **choices,
                           ptrdiff_t num_choices, ChoiceSelection selector, void *user_data);

//! Display a yes/no question on the GInterface, and add a callback for when the
//! choice is selected.
typedef void (*YesNoSelection)(bool);
void ginterface_dpy_yesno(GInterface *interface, const char *prompt,
                          YesNoSelection selector);

//! Display an enemy on the GInterface.
void ginterface_dpy_enemy(GInterface *interface, const unsigned char *img,
                          uint32_t width, uint32_t height);

//! Run an animation on the GInterface.
void ginterface_animation_overlay(GInterface *interface, const Animation *anim);

//! Display a map on the GInterface.
void ginterface_dpy_map(GInterface *interface, const unsigned char *img,
                        uint32_t cells_width, uint32_t cell_height);

//! Set ginterface world.
void ginterface_set_world(GInterface *interface, void *world);

#if __cplusplus
}
#endif

#endif  // CZG_UI_H
