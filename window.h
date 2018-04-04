#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_windows.h>

#include "dialogs.h"

typedef struct
{
	int x, y;
	bool left_key;
} mouse_click;

typedef struct window window;

window* create_window(unsigned width, unsigned height, const char* title);
void destroy_window(window* obj);

void resize_window(window* obj, unsigned width, unsigned height);
bool is_window_destroyed(window* obj);
bool get_input(window* obj, mouse_click* click);

void msg_box(window* obj, const char* title, const char* text);
