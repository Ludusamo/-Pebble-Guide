#ifndef EDITABLENUM_H
#define EDITABLENUM_H

#include <pebble.h>
#include "Util.h"

static void setEditing(TextLayer *layer, bool editing) {
	if (editing) {
		text_layer_set_background_color(layer, GColorBlack);
		text_layer_set_text_color(layer, GColorWhite);
		text_layer_set_text(layer, "34");
	} else {
		text_layer_set_background_color(layer, GColorClear);
		text_layer_set_text_color(layer, GColorBlack);
    text_layer_set_text(layer, "34");
	}
}

#endif //EDITABLENUM_H
