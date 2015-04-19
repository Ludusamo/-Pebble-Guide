#ifndef STUDY_TIME_WINDOW_CREATOR_H
#define STUDY_TIME_WINDOW_CREATOR_H

#include <pebble.h>
#include "EditableNum.h"
#include "Util.h"

static Window *errorWindow;
static TextLayer *error;

static Window *addItemWindow;
static TextLayer *titleLayer;
static TextLayer *editLayer;
int editNum, maxNum, currentEdit;
int timeForNotification;
int entries;

static void setEntries(int e) {
  entries = e;
}

static void createErrorWindow(const char *errorText) {
	errorWindow = window_create();
			
	error = text_layer_create(GRect(0, (168 / 2) - 14, 144, 14)); 
	text_layer_set_background_color(error, GColorClear);
	text_layer_set_text_color(error, GColorBlack);
	text_layer_set_text(error, errorText);
	text_layer_set_font(error, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(error, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(errorWindow), text_layer_get_layer(error));

	window_stack_push(errorWindow, true);
}

static void selectHandler(ClickRecognizerRef recognizer, void *context) {
  if (currentEdit == 0) {
    text_layer_set_text(titleLayer, "Min");
    timeForNotification += editNum * 60;
  } else if (currentEdit == 1) {
    text_layer_set_text(titleLayer, "Dur");
    timeForNotification += editNum;
  } else {
    persist_write_string(entries, "Math");
    persist_write_int(entries + 10, timeForNotification);
    persist_write_int(entries + 20, editNum);
    window_stack_pop(true);
  }
  editNum = 0;
  text_layer_set_text(editLayer, twoDigitToString(editNum));
  currentEdit++;
}

static void upHandler(ClickRecognizerRef recognizer, void *context) {
  if(editNum != maxNum) editNum += 1;
  else editNum = 0;
  text_layer_set_text(editLayer, twoDigitToString(editNum));
}

static void downHandler(ClickRecognizerRef recognizer, void *context) {
  if(editNum != 0) editNum -= 1;
  else editNum = maxNum;
  text_layer_set_text(editLayer, twoDigitToString(editNum));
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, selectHandler);
  window_single_click_subscribe(BUTTON_ID_UP, upHandler);
  window_single_click_subscribe(BUTTON_ID_DOWN, downHandler);
}

static void addItemLoad(Window *window) {
  editNum = 0;
  maxNum = 99;
  
  timeForNotification = 0;
  
  currentEdit = 0;
  
  titleLayer = text_layer_create(GRect(0, 15, 144, 30)); 
	text_layer_set_background_color(titleLayer, GColorClear);
	text_layer_set_text_color(titleLayer, GColorBlack);
	text_layer_set_text(titleLayer, "Hour");
	text_layer_set_font(titleLayer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(titleLayer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(addItemWindow), text_layer_get_layer(titleLayer));
  
  editLayer = text_layer_create(GRect(144 / 2 - 30, 168 / 2 - 15, 60, 30)); 
	text_layer_set_background_color(editLayer, GColorClear);
	text_layer_set_text_color(editLayer, GColorBlack);
	text_layer_set_text(editLayer, twoDigitToString(editNum));
	text_layer_set_font(editLayer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(editLayer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(addItemWindow), text_layer_get_layer(editLayer));
}

static void addItemUnload(Window *window) {
  text_layer_destroy(titleLayer);
	text_layer_destroy(editLayer);
}

static void createAddItemWindow() {
	addItemWindow = window_create();	
  window_set_click_config_provider(addItemWindow, click_config_provider);
  window_set_window_handlers(addItemWindow, (WindowHandlers) {
	    .load = addItemLoad,
	    .unload = addItemUnload,
	});
	window_stack_push(addItemWindow, true);
}

#endif // STUDY_TIME_WINDOW_CREATOR
