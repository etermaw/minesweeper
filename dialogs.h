#pragma once
#include <stdbool.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_windows.h>

#include "config.h"

typedef struct game_params
{
	size_t width, height, mines;
	FIELD_SIZE field_size;
	DIFF_LVL difficulty_lvl;
} game_params;

bool menu_func(ALLEGRO_DISPLAY* wnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result, void *userdata);

void set_dialog_params(game_params* params_ptr, size_t* wins_ptr, size_t* l_ptr);
bool is_new_dialog_msg();






