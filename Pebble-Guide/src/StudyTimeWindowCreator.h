#ifndef STUDY_TIME_WINDOW_CREATOR_H
#define STUDY_TIME_WINDOW_CREATOR_H

#include <pebble.h>
#include "Util.h"

static Window *errorWindow;
static TextLayer *error;

static Window *addItemWindow;
static TextLayer *titleLayer;
static TextLayer *editLayer;
int editNum, maxNum, currentEdit;
int timeForNotification;
int entries;

static WakeupId wakeUp[10];

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
    
    int currentTime = 0;
    time_t curr_time = time(NULL);
    struct tm *tick_time = localtime(&curr_time);
    int currentSeconds = (tick_time->tm_hour * 60 + tick_time->tm_min) * 60;
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", timeForNotification * 60);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", currentSeconds);
    int sec = timeForNotification * 60 - currentSeconds;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", sec);
    time_t future_time = curr_time + sec;

    // Schedule wakeup event and keep the WakeupId
    wakeUp[entries] = wakeup_schedule(future_time, 0, true);
    persist_write_int(entries + 30, wakeUp[entries]);
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
  text_layer_set_text(editLayer, twoDigitToString(editNum));
}

static void clickConfigProvider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, selectHandler);
  window_single_click_subscribe(BUTTON_ID_UP, upHandler);
  window_single_click_subscribe(BUTTON_ID_DOWN, downHandler);
}

static void wakeUpHandler(WakeupId id, int32_t reason) {
  
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
  wakeup_service_subscribe(wakeUpHandler);
  window_set_click_config_provider(addItemWindow, clickConfigProvider);
  window_set_window_handlers(addItemWindow, (WindowHandlers) {
	    .load = addItemLoad,
	    .unload = addItemUnload,
	});
	window_stack_push(addItemWindow, true);
}

#endif // STUDY_TIME_WINDOW_CREATOR
