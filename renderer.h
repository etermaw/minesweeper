#pragma once
#include <allegro5\allegro5.h>

#include "vector.h"

typedef struct renderer renderer;

renderer* create_renderer(const char* atlas_name, ALLEGRO_COLOR color_mask);
void destroy_renderer(renderer* obj);

size_t register_sprite(renderer* obj, int x, int y, int w, int h);
void draw_sprite(renderer* obj, int SID, float x, float y);
