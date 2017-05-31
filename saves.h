#pragma once

#include "game.h"
#include "dialogs.h"

void save_internal_state(game* g, game_params* p, const char* filename);
bool restore_internal_state(game* g, game_params* p, const char* filename);

void load_stats(const char* filename, size_t* wins_arr, size_t* lost_arr);
void save_stats(const char* filename, size_t* wins_arr, size_t* lost_arr);