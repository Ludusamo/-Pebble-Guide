#include <pebble.h>

#define NUM_MENU_SECTIONS 0
#define MAX_ENTRIES 10

static Window *window;
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
	menu_cell_basic_header_draw(ctx, cellLayer, "HEADER");
}

static void drawRow(GContext *ctx, const Layer *cellLayer, MenuIndex *cellIndex, void *data) {
	if (cellIndex->row != 0) {
		menu_cell_basic_draw(ctx, cellLayer, "Add Item...", "", NULL);
	} else {
		char subject[20];
		persist_read_string(cellIndex->row, subject, sizeof(subject));
		menu_cell_basic_draw(ctx, cellLayer, subject, "", NULL);
	}
}

static void select(MenuLayer *menu, MenuIndex *cellIndex, void *data) {

}

static void window_load(Window *window) {
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

static void window_unload(Window *window) {
	menu_layer_destroy(times_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
