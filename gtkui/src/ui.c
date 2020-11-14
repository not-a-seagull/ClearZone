/* gtkui/src/ui.c
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

#include "ui.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "animation.h"
#include "panic.h"
#include "world_shim.h"

#define GI_MAGIC_NUMBER 66666

typedef struct ChoiceResult {
  void *v;
  ptrdiff_t choice;
} ChoiceResult;

//! GInterface actual type.
typedef struct GInterfacePrivate {
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

  //! Current selector function.
  void *selector;
  //! Allocated choice objects.
  ChoiceResult **choices;
  ptrdiff_t num_choices;

  //! Pointer to the world object.
  void *world;

  void *user_data;
 
  int argc;
  char **argv;
} GInterfacePrivate;

//! Function Prototypes
static void activate(GtkApplication *app, gpointer user_data);
void drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void yes_button_click(GtkWidget *widget, gpointer user_data);
void no_button_click(GtkWidget *widget, gpointer user_data);
void choice_select(GtkWidget *widget, gpointer user_data);
void process_yesno_res(GInterfacePrivate *interface, bool yesno);
GInterfacePrivate *verify(GInterface *interface);

//! Initialize the GInterface.
GInterface *ginterface_init(int argc, char **argv) {
  GtkApplication *application;

  // Malloc the memory for the interface.
  GInterfacePrivate *interface = calloc(1, sizeof(GInterfacePrivate));
  interface->magic = GI_MAGIC_NUMBER;
  interface->activated = false;
  interface->argc = argc;
  interface->argv = argv;
  interface->current_drawbox =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, 450, 900);
  interface->world = NULL;
 
  // Fill the surface with white pixels for now.
  cairo_surface_flush(interface->current_drawbox);
  memset(cairo_image_surface_get_data(interface->current_drawbox), 255, 450 * 900 * 4);
  cairo_surface_mark_dirty(interface->current_drawbox);

  // Create the Application.
  application = gtk_application_new("org.not_a_seagull.clear_zone",
                                    G_APPLICATION_FLAGS_NONE);
  g_signal_connect(application, "activate", G_CALLBACK(activate),
                   (gpointer)interface);
  interface->app = application;

  // return interface
  return (GInterface *)interface;
}

int ginterface_run(GInterface *interface) {
  GInterfacePrivate *v = verify(interface);
  return g_application_run(G_APPLICATION(v->app), v->argc, v->argv);
}

void ginterface_drop(GInterface *interface) {
  if (interface) {
    GInterfacePrivate *v = (GInterfacePrivate *)interface;
    if (v->app) {
      g_object_unref(v->app);
    }
  }
}

void ginterface_dpy_text(GInterface *interface, const char *line) {
  GInterfacePrivate *v = verify(interface);
  GtkWidget *label;
  label = gtk_label_new(line);
  gtk_box_pack_start(GTK_BOX(v->textbox), label, FALSE, FALSE, 0);
  printf("Text: %s", line);
  gtk_widget_show(label);
}

void ginterface_dpy_yesno(GInterface *interface, const char *prompt, YesNoSelection selector) {
  ginterface_dpy_text(interface, prompt);
  GInterfacePrivate *v = verify(interface);
  GtkWidget *yesnobox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
  GtkWidget *yes_button = gtk_button_new_with_label("Yes");
  GtkWidget *no_button = gtk_button_new_with_label("No");
  gtk_box_pack_start(GTK_BOX(yesnobox), yes_button, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(yesnobox), no_button,  TRUE, TRUE, 0);
  g_signal_connect(yes_button, "clicked", G_CALLBACK(yes_button_click), (gpointer) v);
  g_signal_connect(no_button,  "clicked", G_CALLBACK(no_button_click),  (gpointer) v);

  gtk_widget_show_all(yesnobox);
  v->choicebox = yesnobox;
  v->selector = selector;
}

void ginterface_dpy_choice(GInterface *interface, const char **choices, ptrdiff_t num_choices, ChoiceSelection selector, void *user_data) {
  GInterfacePrivate *v = verify(interface);
  GtkWidget *choicebox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  GtkWidget *cbutton;
  ChoiceResult *cres;

  // allocate storage
  v->choices = malloc(sizeof(ChoiceResult) * num_choices);
  v->num_choices = num_choices;
  v->user_data = user_data;

  for (ptrdiff_t i = 0; i < num_choices; i++) {
    cbutton = gtk_button_new_with_label(choices[i]); 
    cres = malloc(sizeof(ChoiceResult));
    cres->v = (void *)v;
    cres->choice = i;
    g_signal_connect(cbutton, "clicked", G_CALLBACK(choice_select), cres);
    gtk_box_pack_start(GTK_BOX(choicebox), cbutton, FALSE, FALSE, 0);
    v->choices[i] = cres;
  }
  gtk_box_pack_start(GTK_BOX(v->textbox), choicebox, FALSE, FALSE, 0);
  gtk_widget_show_all(choicebox);
  v->choicebox = choicebox;
  v->selector = selector;
}

void ginterface_set_world(GInterface *interface, void *world) {
  GInterfacePrivate *v = verify(interface);
  v->world = world;
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *parent;
  GtkWidget *texttainer;
  GtkWidget *textbox;
  GtkWidget *drawbox;
  GtkWidget *init_sign;
  GtkWidget *container_box;
  GInterfacePrivate *interface = (GInterfacePrivate *)user_data;

  // Create the parent window
  parent = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(parent), "Clear Zone");
  gtk_window_set_default_size(GTK_WINDOW(parent), 1000, 950);
  gtk_window_set_position(GTK_WINDOW(parent), GTK_WIN_POS_CENTER);

  // Create the box containing both items.
  container_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_container_add(GTK_CONTAINER(parent), container_box);

  // Create a scrolling box
  texttainer = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(texttainer), 10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(texttainer),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_box_pack_start(GTK_BOX(container_box), texttainer, TRUE, TRUE, 0);

  // Create the textbox, a box containing text items.
  textbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(texttainer), textbox);

  init_sign = gtk_label_new("Initializing ClearZone...");
  gtk_box_pack_start(GTK_BOX(textbox), init_sign, FALSE, FALSE, 0);

  // Create the drawbox, corresponding to an image surface.
  drawbox = gtk_drawing_area_new();
  g_signal_connect(G_OBJECT(drawbox), "draw", G_CALLBACK(drawing_area_draw),
                   interface);
  gtk_box_pack_start(GTK_BOX(container_box), drawbox, TRUE, TRUE, 0);

  interface->parent = parent;
  interface->textbox = textbox;
  interface->drawbox = drawbox;

  gtk_widget_show_all(parent);
  ginterface_dpy_text(interface, "Welcome to ClearZone!\n");
}

GInterfacePrivate *verify(GInterface *interface) {
  GInterfacePrivate *v = (GInterfacePrivate *)interface;
  assert(v->magic == GI_MAGIC_NUMBER, "Invalid GInterface");
  return v;
}

void drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  GInterfacePrivate *interface = (GInterfacePrivate *)user_data;
  cairo_set_source_surface(cr, interface->current_drawbox, 10, 10);
  cairo_paint(cr);
}

void yes_button_click(GtkWidget *widget, gpointer user_data) {
  GInterfacePrivate *interface = (GInterfacePrivate *)user_data;
  process_yesno_res(interface, true);
}

void no_button_click(GtkWidget *widget, gpointer user_data) {
  GInterfacePrivate *interface = (GInterfacePrivate *)user_data;
  process_yesno_res(interface, true);
}

void process_yesno_res(GInterfacePrivate *interface, bool yesno) {
  // delete the part
  gtk_widget_hide(interface->choicebox);
  g_object_unref(interface->choicebox);
  interface->choicebox = NULL;

  // selector
  ((YesNoSelection) interface->selector)(yesno);
  interface->selector = NULL;
}

void choice_select(GtkWidget *widget, gpointer user_data) {
   ChoiceResult *cr = (ChoiceResult *) user_data;
   GInterfacePrivate *interface = cr->v;
   ptrdiff_t choice = cr->choice;

   // delete the choices
   for (ptrdiff_t i = 0; i < interface->num_choices; i++) {
     free(interface->choices[i]);
   }
   free(interface->choices);

  // hide the box
  gtk_widget_hide(interface->choicebox);
  g_object_unref(interface->choicebox);
  interface->choicebox = NULL;
 
  // selector
  ((ChoiceSelection) interface->selector)(choice, (GInterface *) interface, interface->user_data);
  interface->selector = NULL;
  interface->user_data = NULL;
}
