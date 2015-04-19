#include <pebble.h>
#include <Flashcards.h>

#ifndef FLASHCARDLIST_H
#define FLASHCARDLIST_H
#define NUM_FLASHCARDLIST_ITEMS 5
#define NUM_FLASHCARDLIST_SECTIONS 1
  
static Window *s_flashcardlist_window;
static SimpleMenuLayer *s_flashcardlist_layer;

static SimpleMenuSection s_flashcardlist_sections[NUM_FLASHCARDLIST_SECTIONS];
static SimpleMenuItem s_flashcardlist_items[NUM_FLASHCARDLIST_ITEMS];

static void flashcardlist_menu_select_callback(int index, void *ctx) {
  flashcards_init(index);
}

static void flashcardlist_window_load(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;
  
  s_flashcardlist_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Periodic Elements",
    .callback = flashcardlist_menu_select_callback,
  };
  s_flashcardlist_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Spanish Vocab",
    .subtitle = "",
    .callback = flashcardlist_menu_select_callback,
  };

  s_flashcardlist_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FLASHCARDLIST_ITEMS,
    .items = s_flashcardlist_items,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_flashcardlist_layer = simple_menu_layer_create(bounds, window, s_flashcardlist_sections, NUM_FLASHCARDLIST_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_flashcardlist_layer));
}

void flashcardlist_window_unload(Window *window) {
  simple_menu_layer_destroy(s_flashcardlist_layer);
}

static void flashcardlist_init() {
  s_flashcardlist_window = window_create();
  window_set_window_handlers(s_flashcardlist_window, (WindowHandlers) {
    .load = flashcardlist_window_load,
    .unload = flashcardlist_window_unload,
  });
  window_stack_push(s_flashcardlist_window, true);
}

  #endif