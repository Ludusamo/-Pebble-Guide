#include "pebble.h"
#include "FlashcardList.h"
#include "About.h"
#include "StudyTimeWindowCreator.h"
#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3
#define MAX_ENTRIES 10

static Window *splashWindow;
static GBitmap *splashImage;
static BitmapLayer *bitmapLayer;
static TextLayer *bitmapText;

static Window *s_main_window;
static Window *studyTimeWindow;
static MenuLayer *times_layer;

static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];

int numEntries;

static uint16_t getNumSections(MenuLayer *menuLayer, void *data) {
	return NUM_MENU_SECTIONS;
}

static uint16_t getNumRows(MenuLayer *menu, uint16_t sectionIndex, void *data) {
	return numEntries + 1;		
}

static int16_t getHeaderHeight(MenuLayer *menu, uint16_t sectionIndex, void *data) {
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void drawHeader(GContext *ctx, const Layer *cellLayer, uint16_t sectionIndex, void * data) {
	menu_cell_basic_header_draw(ctx, cellLayer, "Study Times");
}

static void drawRow(GContext *ctx, const Layer *cellLayer, MenuIndex *cellIndex, void *data) {
	if (cellIndex->row == 0) {
		menu_cell_basic_draw(ctx, cellLayer, "Add Item...", "", NULL);
	} else {
		char subject[20];
		persist_read_string(cellIndex->row - 1, subject, sizeof(subject));
		int time = persist_read_int(10 + cellIndex->row - 1);
		
		menu_cell_basic_draw(ctx, cellLayer, subject, minuteToTime(time), NULL);
	}
}

static void select(MenuLayer *menu, MenuIndex *cellIndex, void *data) {
	if (cellIndex->row == 0) {
		if (numEntries > 10) {
			createErrorWindow("Too Many Entries...");	
			return;
		} else {
			setEntries(numEntries);
      numEntries++;
      createAddItemWindow();
		}
	} else {
    for (int i = cellIndex->row; i < numEntries; i++) {
      char buffer[20];
      persist_read_string(i + 1, buffer, sizeof(buffer));
      persist_write_string(i, buffer);
      persist_write_int(i + 10, persist_read_int(i + 11));
      persist_write_int(i + 20, persist_read_int(i + 21));
      persist_write_int(i + 30, persist_read_int(i + 31));
    }
    numEntries--;
    menu_layer_reload_data(times_layer);
	}
}

static void createStudyTimes(Window *window) {
	// Check num entries
	for (int i = 0; i < MAX_ENTRIES; i++) {
		if (persist_exists(i)) {
			numEntries = i + 1;	
		}	
	}

  	Layer *window_layer = window_get_root_layer(window);
  	GRect bounds = layer_get_bounds(window_layer);

	times_layer = menu_layer_create(bounds);
	menu_layer_set_callbacks(times_layer, NULL, (MenuLayerCallbacks) {
	.get_num_sections = getNumSections,
	.get_num_rows = getNumRows,
	.get_header_height = getHeaderHeight,
	.draw_header = drawHeader,
	.draw_row = drawRow,
	.select_click = select,
	});

	menu_layer_set_click_config_onto_window(times_layer, window);
	layer_add_child(window_layer, menu_layer_get_layer(times_layer));
}

static void unloadStudyTimes(Window *window) {
	menu_layer_destroy(times_layer);
	text_layer_destroy(error);
}

static void menu_select_callback(int index, void *ctx) {
  if(index == 0){
    flashcardlist_init();
  }
  else if(index == 1){
    studyTimeWindow = window_create();
	  window_set_window_handlers(studyTimeWindow, (WindowHandlers) {
	    .load = createStudyTimes,
	    .unload = unloadStudyTimes,
	  });
    window_stack_push(studyTimeWindow, true);
  }
  else if(index == 2){
    about_init();
  }
  layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));
}


static void main_window_load(Window *window) {

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Flashcards",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Study Times",
    .subtitle = "",
    .callback = menu_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "About",
    .subtitle = "",
    .callback = menu_select_callback,
  };

 
  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
}

static void enterApp() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void init() {
  enterApp();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}