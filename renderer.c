#include <stdlib.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>

#include "renderer.h"

struct renderer
{
	ALLEGRO_BITMAP* atlas;
	vector* sprites;
};

renderer* create_renderer(const char* atlas_name, ALLEGRO_COLOR color_mask)
{
	renderer* tmp = (renderer*)malloc(sizeof(renderer));
	
	tmp->atlas = al_load_bitmap(atlas_name);
	al_convert_mask_to_alpha(tmp->atlas, color_mask);

	tmp->sprites = create_vector(sizeof(ALLEGRO_BITMAP*));

	return tmp;
}

void destroy_renderer(renderer* obj)
{
	while(size(obj->sprites) != 0)
	{
		ALLEGRO_BITMAP** sprite_to_delete = (ALLEGRO_BITMAP**)pop_back(obj->sprites);
		al_destroy_bitmap(*sprite_to_delete);
	}

	al_destroy_bitmap(obj->atlas);

	destroy_vector(obj->sprites);

	free(obj);
}

size_t register_sprite(renderer* obj, int x, int y, int w, int h)
{
	ALLEGRO_BITMAP** sprite = (ALLEGRO_BITMAP**)push_back(obj->sprites);

	*sprite = al_create_sub_bitmap(obj->atlas, x, y, w, h);
	
	return size(obj->sprites);
}

void draw_sprite(renderer* obj, int SID, float x, float y)
{
	ALLEGRO_BITMAP** spr = (ALLEGRO_BITMAP**)item_ref(obj->sprites, SID);
	
	al_draw_bitmap(*spr, x, y, 0);
}