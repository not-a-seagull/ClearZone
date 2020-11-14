/* gtkui/src/animation.c
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

#include "animation.h"

#include <stdlib.h>

#include "panic.h"
#include "stb_image.h"

Animation *animation_new(const char *fname1, const char *fname2, bool *frames,
                         ptrdiff_t frames_len) {
  int width1, height1, channels1, width2, height2, channels2;

  assert(fname1, "First file name is null");
  assert(frames, "Frames are null");
  assert(frames_len > 0, "Frames must have at least one length");

  unsigned char *img1 = stbi_load(fname1, &width1, &height1, &channels1, 0);
  assert(img1, "Failed to load image of animation");
  unsigned char *img2;

  if (fname2) {
    stbi_load(fname2, &width2, &height2, &channels2, 0);
    assert(img2, "Failed to load image of animation");
    assert(width1 == width2 && height1 == height2,
           "Animations do not match in terms of frames");
  }

  Animation *anim = malloc(sizeof(Animation));
  anim->frame1 = img1;
  anim->frame2 = img2;
  anim->width = width1;
  anim->height = height1;
  anim->frames = frames;
  anim->frames_len = frames_len;

  return anim;
}

void animation_drop(Animation *anim) {
  if (anim) {
    if (anim->frame1) {
      free(anim->frame1);
    }
    if (anim->frame2) {
      free(anim->frame2);
    }
    if (anim->frames) {
      free(anim->frames);
    }
  }
}
