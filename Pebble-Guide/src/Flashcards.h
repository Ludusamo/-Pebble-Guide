#include <pebble.h>
#include <ScienceFlashcards.h> 
#include <SpanishFlashcards.h>
#include <string.h>

static Window *s_flashcards_window;
static TextLayer *text_layer;

static int set;
static unsigned int index;
static unsigned int limit;
static bool isKeyword;
static char *current;

static char *keywords;
static char *definitions;

static char *substring(char* array, int start){
  char *buffer = "          ";
  for(int i = 0; i < 10; i++){
    buffer[i] = array[start+i];
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", buffer);
  return buffer;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(isKeyword){
    current = substring(definitions, index);
    isKeyword = false;
  }
  else{
    current = substring(keywords, index);
    isKeyword = true;
  }
  text_layer_set_text(text_layer, current);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(index < limit-10){
    index+=10;
    current = substring(keywords, index);
    text_layer_set_text(text_layer, current);
    
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(index > 0){
    index -= 10;
    current = substring(keywords, index);
    text_layer_set_text(text_layer, current);
    
 }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void flashcards_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Start Edit Here
 
  if(set == 0){
    science_flashcards_init();
    keywords = science_keywords;
    definitions = science_definitions;
  }
  if(set == 1){
    spanish_flashcards_init();
    keywords = spanish_keywords;
    definitions = spanish_definitions;
  }
  
  
  index = 0;
  limit = strlen(keywords);
  isKeyword = true;
  
  current = substring(keywords, index);
  
  text_layer = text_layer_create((GRect) { .origin = { 10, 50 }, .size = { bounds.size.w, 40 } });
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text(text_layer, current);
  //End Edit Here
  
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void flashcards_window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void flashcards_init(int num) {
  set = num;
  s_flashcards_window = window_create();
  window_set_click_config_provider(s_flashcards_window, click_config_provider);
  window_set_window_handlers(s_flashcards_window, (WindowHandlers) {
    .load = flashcards_window_load,
    .unload = flashcards_window_unload,
  });
  window_stack_push(s_flashcards_window, true);
}

  
