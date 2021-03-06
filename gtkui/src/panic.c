/* gtkui/src/panic.c
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void panic(const char *msg) {
  fprintf(stderr,
          "A fatal error has occured and ClearZone must shut down: %s\n", msg);
  fprintf(stderr, "System Error: %s\n", strerror(errno));
  exit(1);
}

void assert(int cond, const char *msg) {
  if (!cond) {
    panic(msg);
  }
}
