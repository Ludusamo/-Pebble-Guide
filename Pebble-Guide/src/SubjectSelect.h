#ifndef SUBJECTSELECT_H
#define SUBJECTSELECT_H

#include <pebble.h>
#include "StudyTimeWindowCreator.h"
    
#define NUM_SUBJECT_ITEMS 6
#define NUM_SUBJECT_SECTIONS 1
  
static Window *subjectSelectWindow;
static SimpleMenuLayer *subjects;

static SimpleMenuSection subjectsSections[NUM_SUBJECT_SECTIONS];
static SimpleMenuItem subjectsItems[NUM_SUBJECT_ITEMS];

static void subjectSelect(int index, void *ctx) {
  setSubject(index);
  createAddItemWindow();
}

static void subjectWindowLoad(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;
  
  subjectsItems[num_a_items++] = (SimpleMenuItem) {
    .title = "Math",
    .callback = subjectSelect,
  };
  subjectsItems[num_a_items++] = (SimpleMenuItem) {
    .title = "Spanish",
    .subtitle = "",
    .callback = subjectSelect,
  };
  subjectsItems[num_a_items++] = (SimpleMenuItem) {
    .title = "Chemistry",
    .subtitle = "",
    .callback = subjectSelect,
  };
  subjectsItems[num_a_items++] = (SimpleMenuItem) {
    .title = "History",
    .subtitle = "",
    .callback = subjectSelect,
  };
  subjectsItems[num_a_items++] = (SimpleMenuItem) {
    .title = "English",
    .subtitle = "",
    .callback = subjectSelect,
  };
  subjectsItems[num_a_items++] = (SimpleMenuItem) {
    .title = "Misc.",
    .subtitle = "",
    .callback = subjectSelect,
  };

  subjectsSections[0] = (SimpleMenuSection) {
    .num_items = NUM_SUBJECT_ITEMS,
    .items = subjectsItems,
  };
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  subjects = simple_menu_layer_create(bounds, window, subjectsSections, NUM_SUBJECT_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(subjects));
}

void subjectWindowUnload(Window *window) {
  simple_menu_layer_destroy(subjects);
}

static void subjectListInit() {
  subjectSelectWindow = window_create();
  window_set_window_handlers(subjectSelectWindow, (WindowHandlers) {
    .load = subjectWindowLoad,
    .unload = subjectWindowUnload,
  });
  window_stack_push(subjectSelectWindow, true);
}

  #endif