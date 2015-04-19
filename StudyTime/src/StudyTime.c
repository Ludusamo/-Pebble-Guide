#include <pebble.h>
#include "StudyTimeWindowCreator.h"
#include "Util.h"

#define NUM_MENU_SECTIONS 0
#define MAX_ENTRIES 10

static Window *studyTimeWindow;
static MenuLayer *times_layer;

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

static void unloadStudyTimes(Window *window) {
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

static void createStudyTimes(Window *window) {
	menu_layer_destroy(times_layer);
	text_layer_destroy(error);
}

static void init(void) {
	  studyTimeWindow = window_create();
	  window_set_window_handlers(studyTimeWindow, (WindowHandlers) {
	    .load = createStudyTimes,
	    .unload = unloadStudyTimes,
	  });
	  window_stack_push(studyTimeWindow, true);
}

static void deinit(void) {
  	window_destroy(studyTimeWindow);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
