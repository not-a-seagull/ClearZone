/* gtkui/src/ui.cpp
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

#include <optional>

#include "animation.h"
#include "panic.h"
#include "world_shim.h"

#define GI_MAGIC_NUMBER 66666

//! Function Prototypes
static void activate(GtkApplication *app, gpointer user_data);
void drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void yes_button_click(GtkWidget *widget, gpointer user_data);
void no_button_click(GtkWidget *widget, gpointer user_data);
void choice_select(GtkWidget *widget, gpointer user_data);
void process_yesno_res(GInterface *interface, bool yesno);

const int SURFACE_WIDTH = 450;
const int SURFACE_HEIGHT = 900;

//! Initialize the GInterface.
GInterface::GInterface(int argc, char **argv) {
  GtkApplication *application;

  // Malloc the memory for the interface.
  this->magic = GI_MAGIC_NUMBER;
  this->activated = false;
  this->argc = argc;
  this->argv = argv;
  this->current_drawbox =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, 450, 900);
  this->world = NULL;

  // Fill the surface with white pixels for now.
  cairo_surface_flush(this->current_drawbox);
  memset(cairo_image_surface_get_data(this->current_drawbox), 255,
         450 * 900 * 4);
  cairo_surface_mark_dirty(this->current_drawbox);

  // Create the Application.
  application = gtk_application_new("org.not_a_seagull.clear_zone",
                                    G_APPLICATION_FLAGS_NONE);
  g_signal_connect(application, "activate", G_CALLBACK(activate),
                   (gpointer)this);
  this->app = application;
}

int GInterface::run() {
  return g_application_run(G_APPLICATION(this->app), this->argc, this->argv);
}

GInterface::~GInterface() {}

void GInterface::dpy_text(const char *line) {
  GtkWidget *label;
  label = gtk_label_new(line);
  gtk_box_pack_start(GTK_BOX(this->textbox), label, FALSE, FALSE, 0);
  printf("Text: %s", line);
  gtk_widget_show(label);
}

void GInterface::dpy_yesno(const char *prompt,
                           std::function<void(bool)> reply) {
  this->dpy_text(prompt);

  // move reply into user data
  this->yesno_responder = std::optional(reply);

  GtkWidget *yesnobox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
  GtkWidget *yes_button = gtk_button_new_with_label("Yes");
  GtkWidget *no_button = gtk_button_new_with_label("No");
  gtk_box_pack_start(GTK_BOX(yesnobox), yes_button, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(yesnobox), no_button, TRUE, TRUE, 0);
  g_signal_connect(yes_button, "clicked", G_CALLBACK(yes_button_click),
                   (gpointer)this);
  g_signal_connect(no_button, "clicked", G_CALLBACK(no_button_click),
                   (gpointer)this);

  gtk_widget_show_all(yesnobox);
  this->choicebox = yesnobox;
}

void GInterface::dpy_choice(std::shared_ptr<std::string[]> choices,
                            ptrdiff_t num_choices,
                            std::function<void(ptrdiff_t)> reply) {
  this->choice_responder = std::optional(reply);

  printf("Choice:\n");
  GtkWidget *cbutton;
  GtkWidget *choicebox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);

  this->choices = std::make_unique<std::unique_ptr<ChoiceResult>[]>(num_choices);

  for (ptrdiff_t i = 0; i < num_choices; i++) {
    printf(" - %s\n", choices[i].c_str());
    cbutton = gtk_button_new_with_label(choices[i].c_str());
    std::unique_ptr<ChoiceResult> cres = std::unique_ptr<ChoiceResult>(new ChoiceResult(this, i));
    g_signal_connect(cbutton, "clicked", G_CALLBACK(choice_select), cres.get());
    gtk_box_pack_start(GTK_BOX(choicebox), cbutton, FALSE, FALSE, 0);
    this->choices[i] = std::move(cres);
  }
  this->num_choices = num_choices;

  gtk_box_pack_start(GTK_BOX(this->textbox), choicebox, FALSE, FALSE, 0);
  gtk_widget_show_all(choicebox);
  this->choicebox = choicebox;
}

void GInterface::stop() { gtk_main_quit(); }

void GInterface::dpy_map(std::shared_ptr<char[]> img, int px, int py) {
  printf("Updating map...\n");
  // map should be 32x32
  float r, g, b;
  cairo_t *cr = cairo_create(this->current_drawbox);

  const int CELLS_X = 32;
  const int CELLS_Y = 32;
  int cell_width = SURFACE_WIDTH / CELLS_X;
  int cell_height = SURFACE_HEIGHT / CELLS_Y;

  for (int i = 0; i < CELLS_X; i++) {
    for (int j = 0; j < CELLS_Y; j++) {
      // figure out source
      char current = img[(j * CELLS_Y) + i];
      switch (current) {
        case 'F':
          r = 0.0f;
          g = 0.5f;
          b = 0.0f;
          break;
        default:
          r = 0.0f;
          g = 0.0f;
          b = 0.0f;
          break;
      }

      cairo_set_source_rgb(cr, r, g, b);

      int x = cell_width * i;
      int y = cell_height * j;
      cairo_rectangle(cr, x, y, x + cell_width, y + cell_height);
      cairo_fill(cr);
    }
  }

  cairo_set_line_width(cr, 0.6);
  cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
  cairo_move_to(cr, cell_width * px, cell_height * py);
  cairo_line_to(cr, cell_width * (px + 1), cell_height * (py + 1));
  cairo_move_to(cr, cell_width * px, cell_height * (py + 1));
  cairo_line_to(cr, cell_width * (px + 1), cell_height * py);

  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    panic("Cairo failed!");
  }

  cairo_surface_flush(this->current_drawbox);
  cairo_destroy(cr);
  gtk_widget_queue_draw(this->drawbox);
}

void GInterface::initialize_world() {
  std::shared_ptr<std::string[]> init_choices = std::shared_ptr<std::string[]>(new std::string[2]);
  init_choices[0] = "Create New World";
  init_choices[1] = "Load from File"; 

  this->dpy_choice(init_choices, 2, [this](ptrdiff_t load) {
    if (load == 0) {
      this->world = std::unique_ptr<World>(new World());
    } else {
      panic("Unable to load world from file yet");
    }
  });
}

void GInterface::world_tick() {
  printf("Running world tick...\n");
  std::unique_ptr<Event> ev = this->world->next_event();

  int ty = ev->get_type();
  if (ty == TEXT_EVENT_TYPE) {
    TextEvent *tev = dynamic_cast<TextEvent *>(ev.get());
    this->dpy_text(tev->get_text().c_str());
    this->world_tick();
  } else if (ty == YESNO_EVENT_TYPE) {
    YesNoEvent *yesno = dynamic_cast<YesNoEvent *>(ev.get());
    this->dpy_yesno(yesno->get_prompt().c_str(),
                    std::move(yesno->get_responder()));
  } else if (ty == CHOICE_EVENT_TYPE) {
    ChoiceEvent *choice = dynamic_cast<ChoiceEvent *>(ev.get());
    this->dpy_choice(choice->get_choices(), choice->get_num_choices(), std::move(choice->get_responder()));
  } else if (ty == MAPDPY_EVENT_TYPE) {
    MapDpyEvent *mde = dynamic_cast<MapDpyEvent *>(ev.get());
    this->dpy_map(mde->get_data(), mde->get_x(), mde->get_y());
    this->world_tick();
  } else {
    this->stop();
    exit(0);
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *parent;
  GtkWidget *texttainer;
  GtkWidget *textbox;
  GtkWidget *drawbox;
  GtkWidget *container_box;
  GInterface *interface = (GInterface *)user_data;

  // Create the parent window
  parent = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(parent), "Clear Zone");
  gtk_window_set_default_size(GTK_WINDOW(parent), 1000, 950);
  gtk_window_set_position(GTK_WINDOW(parent), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(parent), FALSE);

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

  // Create the drawbox, corresponding to an image surface.
  drawbox = gtk_drawing_area_new();
  g_signal_connect(G_OBJECT(drawbox), "draw", G_CALLBACK(drawing_area_draw),
                   interface);
  gtk_box_pack_start(GTK_BOX(container_box), drawbox, TRUE, TRUE, 0);

  interface->parent = parent;
  interface->textbox = textbox;
  interface->drawbox = drawbox;

  gtk_widget_show_all(parent);
  interface->dpy_text("Welcome to ClearZone!\n");

  interface->initialize_world();
}

void drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  GInterface *interface = (GInterface *)user_data;
  cairo_set_source_surface(cr, interface->current_drawbox, 10, 10);
  cairo_paint(cr);
}

void yes_button_click(GtkWidget *widget, gpointer user_data) {
  GInterface *interface = (GInterface *)user_data;
  process_yesno_res(interface, true);
}

void no_button_click(GtkWidget *widget, gpointer user_data) {
  GInterface *interface = (GInterface *)user_data;
  process_yesno_res(interface, false);
}

void process_yesno_res(GInterface *interface, bool yesno) {
  // delete the part
  gtk_widget_hide(interface->choicebox);

  // selector
  std::function<void(bool)> rep = std::move(interface->yesno_responder).value();
  rep(yesno);

  interface->world_tick();
}

void choice_select(GtkWidget *widget, gpointer user_data) {
  ChoiceResult *cr = (ChoiceResult *)user_data;
  GInterface *interface = cr->v;
  ptrdiff_t choice = cr->choice;

  // hide the box
  gtk_widget_hide(interface->choicebox);

  // selector
  std::function<void(ptrdiff_t)> rep =
      std::move(interface->choice_responder).value();
  rep(choice);

  interface->world_tick();
}
