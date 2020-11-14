/* gtkui/src/world_shim.cpp
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

#include "panic.h"
#include "ui.h"
#include "world.h"
#include "world_shim.h"

#include <cstdlib>
#include <memory>

extern "C" void initialize_world(GInterface *interface) {
    const char **CHOICES = {
      "Create new world",
      "Load from File",
    };
 
    // ask for a name
    ginterface_dpy_text(interface, "Load a world from save, or being a new one?");
    ginterface_dpy_choice(interface, CHOICES, 2, _respond_to_choice);
}

extern "C" void _respond_to_choice(ptrdiff_t choice, GInterface *interface, void *user_data) {
    if (choice == 0) {
        World *world = new World();
        ginterface_set_world((void *) world);
    } else {
        panic("Cannot yet laoad file")
    }
}
