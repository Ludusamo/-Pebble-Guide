#include <pebble.h>

static Window *s_about_window;
static TextLayer *title_layer;
static TextLayer *text_layer;

static void s_about_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  title_layer = text_layer_create(GRect(0, 15, 144, 30)); 
  text_layer_set_text(title_layer, "Pebble Guide");
  text_layer = text_layer_create(GRect(0, 33, 144, 168 - 33)); 
  text_layer_set_text(text_layer, "This companion app is intended to help students study. Created by Joon Lee and Brendan Horng. Sparkling Water Co");
  
  layer_add_child(window_layer, text_layer_get_layer(title_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void s_about_window_unload(Window *window){
  text_layer_destroy(title_layer);
  text_layer_destroy(text_layer);
}
static void about_init(){
  s_about_window = window_create();
  window_set_window_handlers(s_about_window, (WindowHandlers) {
    .load = s_about_window_load,
    .unload = s_about_window_unload,
  });
  window_stack_push(s_about_window, true);
}