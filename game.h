#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "renderer.h"

typedef struct game game;

game* create_game();
void destroy_game(game* obj);

void reset_game_field_params(game* obj, unsigned width, unsigned height, unsigned mines);
void reset_game(game* obj);

void click(game* obj, int x, int y, bool left_key);

bool is_won(game* obj);
bool is_lost(game* obj);

void populate_field(game* obj, int sx, int sy, int(*rng_func)());
void set_field_values(game* obj);

unsigned field_width(game* obj);
unsigned field_height(game* obj);
unsigned mines_left(game* obj);

void save_gamestate(game* obj, FILE* save_file);
void restore_gamestate(game* obj, FILE* save_file);

void draw_game_field(game* obj, renderer* rend);
void draw_game_over(game* obj, renderer* rend);


