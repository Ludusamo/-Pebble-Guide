#ifndef MOTIVATION_H
#define MOTIVATION_H
#define NUM_MOTIVATION_ITEMS 5
#define NUM_MOTIVATION_SECTIONS 1
  
#include <pebble.h>
#include <Quotes.h>
  
static Window *s_motivation_window;
static SimpleMenuLayer *s_motivation_layer;

static char *person1;
static char *person2;
static char *person3;
static char *person4;
static char *person5;
static char *choice;

static SimpleMenuSection s_motivation_sections[NUM_MOTIVATION_SECTIONS];
static SimpleMenuItem s_motivation_items[NUM_MOTIVATION_ITEMS];

static void motivation_menu_select_callback(int index, void *ctx) {
  if(index == 0){
    choice = person1;
  }
  else if(index == 1){
    choice = person2;
  }
  else if(index == 2){
    choice = person3;
  }
  else if(index == 3){
    choice = person4;
  }
  else if(index == 4){
    choice = person5;
  }
  quotes_init(choice, index);
}

static void motivation_window_load(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;
  person1 = "Aristotle";
  person2 = "Carl Rogers";
  person3 = "Nelson Mandela";
  person4 = "Mark Twain";
  person5 = "John Dewey";
  
  s_motivation_items[num_a_items++] = (SimpleMenuItem) {
    .title = person1,
    .callback = motivation_menu_select_callback,
  };
  s_motivation_items[num_a_items++] = (SimpleMenuItem) {
    .title = person2,
    .subtitle = "",
    .callback = motivation_menu_select_callback,
  };
  s_motivation_items[num_a_items++] = (SimpleMenuItem) {
    .title = person3,
    .subtitle = "",
    .callback = motivation_menu_select_callback,
  };

  s_motivation_items[num_a_items++] = (SimpleMenuItem) {
    .title = person4,
    .subtitle = "",
    .callback = motivation_menu_select_callback,
  };
  s_motivation_items[num_a_items++] = (SimpleMenuItem) {
    .title = person5,
    .subtitle = "",
    .callback = motivation_menu_select_callback,
  };
  s_motivation_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_MOTIVATION_ITEMS,
    .items = s_motivation_items,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_motivation_layer = simple_menu_layer_create(bounds, window, s_motivation_sections, NUM_MOTIVATION_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_motivation_layer));
}

void motivation_window_unload(Window *window) {
  simple_menu_layer_destroy(s_motivation_layer);
}

static void motivation_init() {
  s_motivation_window = window_create();
  window_set_window_handlers(s_motivation_window, (WindowHandlers) {
    .load = motivation_window_load,
    .unload = motivation_window_unload,
  });
  window_stack_push(s_motivation_window, true);
}

  #endif