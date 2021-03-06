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
#include "cstring"

#define GI_MAGIC_NUMBER 66666

//! Function Prototypes
static void activate(GtkApplication *app, gpointer user_data);
void drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void yes_button_click(GtkWidget *widget, gpointer user_data);
void no_button_click(GtkWidget *widget, gpointer user_data);
void choice_select(GtkWidget *widget, gpointer user_data);
void process_yesno_res(GInterface *interface, bool yesno);
void print_inventory(GtkWidget *widget, gpointer user_data);

const int SURFACE_WIDTH = 650;
const int SURFACE_HEIGHT = 900;

const int MAP_WIDTH = 200, MAP_HEIGHT = 200, OCTAVES = 5; 
const float MAP_SCALE = 1.33f;
const int SEED = 353865;

//! Initialize the GInterface.
GInterface::GInterface(int argc, char **argv) {
  GtkApplication *application;

  // Malloc the memory for the interface.
  this->magic = GI_MAGIC_NUMBER;
  this->activated = false;
  this->argc = argc;
  this->argv = argv;
  this->current_drawbox =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, SURFACE_WIDTH, SURFACE_HEIGHT);
  this->world = NULL;

  // Fill the surface with white pixels for now.
  cairo_surface_flush(this->current_drawbox);
  memset(cairo_image_surface_get_data(this->current_drawbox), 255,
         SURFACE_WIDTH * SURFACE_HEIGHT * 4);
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

void scrl(GtkWidget *tbox) {
  GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(tbox));
  double lower = gtk_adjustment_get_upper(adj);
  gtk_adjustment_set_value(adj, lower);
//  gtk_widget_set_size_request(tbox, 300, 950);
}

void GInterface::dpy_text(const char *line) {
  GtkWidget *label;
  label = gtk_label_new(line);
  gtk_box_pack_start(GTK_BOX(this->textbox), label, FALSE, FALSE, 0);
  printf("Text: %s", line);
  gtk_widget_show(label);
  scrl(this->scrollbox);
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
  scrl(this->scrollbox);
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

  gtk_box_pack_start(GTK_BOX(this->textbox), choicebox, TRUE, FALSE, 0);
  gtk_widget_show_all(choicebox);
  this->choicebox = choicebox;
  scrl(this->scrollbox);
}

void GInterface::stop() { gtk_main_quit(); }

void GInterface::dpy_map(std::shared_ptr<char[]> img, int px, int py) {
  printf("Updating map...\n");
  // map should be 32x32
  float r, g, b;
  cairo_t *cr = cairo_create(this->current_drawbox);

  const int CELLS_X = 32;
  const int CELLS_Y = 32;
  double cell_width = SURFACE_WIDTH / CELLS_X;
  double cell_height = SURFACE_HEIGHT / CELLS_Y;

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
        case 'D':
          r = 1.0f;
          g = 0.5f;
          b = 0.0f;
          break;
        case 'P':
          r = 0.0f;
          g = 0.9f;
          b = 0.2f;
          break;
        case 'J':
          r = 0.1f;
          g = 0.5f;
          b = 0.0f;
          break;
        case 'A':
          r = 0.5f;
          g = 0.5f; 
          b = 0.5f;
          break;
        case 'W':
          r = 0.0f;
          g = 0.2f;
          b = 0.6f;
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
      cairo_rectangle(cr, x, y, cell_width, cell_height);
      cairo_fill(cr);
    }
  }

  double offset_x = (double)cell_width * 0.1;
  double offset_y = (double)cell_height * 0.1;

  cairo_set_source_rgb(cr, 0.4, 0.4, 0.9);
  cairo_rectangle(cr, (px * cell_width) + offset_x, (py * cell_height) + offset_y, cell_width - (offset_x * 2), cell_height - (offset_y * 2));
  cairo_fill(cr);

  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    panic("Cairo failed!");
  }

  cairo_surface_flush(this->current_drawbox);
  cairo_destroy(cr);
  gtk_widget_queue_draw(this->drawbox);
}

void GInterface::initialize_world(int width, int height, int octaves, int seed, float scale) {
  std::shared_ptr<std::string[]> init_choices = std::shared_ptr<std::string[]>(new std::string[2]);
  init_choices[0] = "Create New World";
  init_choices[1] = "Load from File"; 

  this->dpy_choice(init_choices, 2, [this, width, height, octaves, seed, scale](ptrdiff_t load) {
    if (load == 0) {
      this->world = std::unique_ptr<World>(new World(width, height, octaves, seed, scale));
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
  GtkWidget *leftbox;
  GtkWidget *inv_button;
  GtkWidget *container_box;
  GInterface *interface = (GInterface *)user_data;

  // Create the parent window
  parent = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(parent), "Clear Zone");
  gtk_window_set_default_size(GTK_WINDOW(parent), 1200, 950);
  gtk_window_set_position(GTK_WINDOW(parent), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(parent), FALSE);

  // Create the box containing both items.
  container_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_container_add(GTK_CONTAINER(parent), container_box);

  leftbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(container_box), leftbox, TRUE, TRUE, 0);

  // Create a scrolling box
  texttainer = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(texttainer), 10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(texttainer),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_box_pack_start(GTK_BOX(leftbox), texttainer, TRUE, TRUE, 0);

  inv_button = gtk_button_new_with_label("Print Inventory");
  gtk_box_pack_start(GTK_BOX(leftbox), inv_button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(inv_button), "clicked", G_CALLBACK(print_inventory), interface);

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
  interface->scrollbox = texttainer;

  gtk_widget_show_all(parent);
  interface->dpy_text("Welcome to ClearZone!\n");

  interface->initialize_world(MAP_WIDTH, MAP_HEIGHT, OCTAVES, SEED, MAP_SCALE);
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

void print_inventory(GtkWidget *w, gpointer user_data) {
  GInterface *interface = (GInterface *) user_data;
  if (!interface->world) return;
  
  if (interface->choicebox) {
    gtk_widget_hide(interface->choicebox);
  }

  interface->world->printInventory();
  interface->world_tick();

}
