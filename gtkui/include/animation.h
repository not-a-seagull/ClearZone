/* gtkui/include/animation.h
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

#ifndef CZG_ANIMATION_H
#define CZG_ANIMATION_H

#if __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//! Two-frame animation.
typedef struct Animation {
  //! Frame 1
  unsigned char *frame1;
  //! Frame 2
  unsigned char *frame2;
  //! Width and height;
  uint32_t width;
  uint32_t height;
  //! Slice of booleans representing which frames to run.
  bool *frames;
  ptrdiff_t frames_len;
} Animation;

//! Initialize an animation from two files.
Animation *animation_new(const char *fname1, const char *fname2, bool *frames,
                         ptrdiff_t frames_len);
//! Destroy an animation.
void animation_drop(Animation *anim);

#if __cplusplus
}
#endif

#endif
