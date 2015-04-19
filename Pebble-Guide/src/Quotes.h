#include <pebble.h>

static Window *s_quote_window;
static TextLayer *author_layer;
static TextLayer *quote_layer;
static int num;
static char *quote1;
static char *quote2;
static char *quote3;
static char *quote4;
static char *quote5;
static char *author;



static void s_quote_window_load(Window *window){
  
  quote1 = "The roots of education are bitter, but the fruit is sweet.";
  quote2 = "The only person who is educated is the one who has learned how to learn and change.";
  quote3 = "Education is the most powerful weapon which you can use to change the world.";
  quote4 = "In the first place, God made idiots. This was for practice. Then He made school boards.";
  quote5 = "Education is not preparation for life; education is life itself.";
  
  Layer *window_layer = window_get_root_layer(window);
  author_layer = text_layer_create(GRect(0, 15, 144, 30)); 
  quote_layer = text_layer_create(GRect(0, 33, 144, 168 - 33)); 
  
  text_layer_set_text(author_layer, author);
  layer_add_child(window_layer, text_layer_get_layer(author_layer));
  
  if(num == 0){
    text_layer_set_text(quote_layer, quote1);
  }
   else if(num == 1){
    text_layer_set_text(quote_layer, quote2);
  }
   else if(num == 2){
    text_layer_set_text(quote_layer, quote3);
  }
   else if(num == 3){
    text_layer_set_text(quote_layer, quote4);
  }
   else if(num == 4){
    text_layer_set_text(quote_layer, quote5);
  }
  
  
  layer_add_child(window_layer, text_layer_get_layer(quote_layer));
}

static void s_quote_window_unload(Window *window){
  text_layer_destroy(author_layer);
  text_layer_destroy(quote_layer);
}

static void quotes_init(char *person, int index){
  author = person;
  num = index;
  s_quote_window = window_create();
  window_set_window_handlers(s_quote_window, (WindowHandlers) {
    .load = s_quote_window_load,
    .unload = s_quote_window_unload,
  });
  window_stack_push(s_quote_window, true);
}
