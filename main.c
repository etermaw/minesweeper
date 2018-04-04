#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>

#include "game.h"
#include "config.h"
#include "window.h"
#include "renderer.h"
#include "saves.h"

void draw_number(renderer* r, unsigned num, float x, float y)
{
	for (int offset = 32; offset >= 0; offset -= 16)
	{
		draw_sprite(r, 16 + num % 10, x + offset, y);
		num /= 10;
	}
}

#define clamp(value,down,up) ((value) < (down) ? (down) : ((value) > (up) ? (up) : (value)))

void validate(game_params* par)
{
	if (par->field_size != F_USER)
	{
		par->width = field_lookup[par->field_size].size;
		par->height = field_lookup[par->field_size].size;
	}

	if (par->difficulty_lvl != D_USER)
		par->mines = diff_lookup[par->difficulty_lvl].size;

	par->width = clamp(par->width, MIN_FIELD_WIDTH, MAX_FIELD_WIDTH);
	par->height = clamp(par->height, MIN_FIELD_HEIGHT, MAX_FIELD_HEIGHT);
	par->mines = clamp(par->mines, MIN_MINES, min(MAX_MINES,par->width * par->height * 0.95));
}

#undef clamp()

int main()
{
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	srand(time(NULL));

	game* g = create_game();
	window* w = create_window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Minesweeper");
	renderer* r = create_renderer("sprites.png", al_map_rgb(255, 0, 255));

	game_params p = { DEFAULT_FIELD_WIDTH, DEFAULT_FIELD_HEIGHT, DEFAULT_MINES, F_SMALL, D_EASY };
	size_t won[8] = { 0 },
		   lost[8] = { 0 };

	clock_t start_time = 0;
	mouse_click c;
	bool first_click = true;

	//load sprites
	for (int i = 0; i < 16; ++i)
		register_sprite(r, i * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE);

	for (int i = 0; i < 10; ++i)
		register_sprite(r, i * CELL_SIZE, 16, CELL_SIZE, CELL_SIZE);

	//try to load statistics and previous game
	load_stats(STATS_PATH, won, lost);
	
	if (restore_internal_state(g, &p, SAVE_PATH))
		first_click = false;

	//set up game and window
	set_dialog_params(&p, won, lost);
	resize_window(w, field_width(g) * CELL_SIZE, field_height(g) * CELL_SIZE + DOWN_BORDER);

	while (!is_window_destroyed(w))
	{
		if (is_won(g))
		{
			msg_box(w, "You won!", "Congratulations, you won!");
			reset_game(g);
			first_click = true;

			if(p.difficulty_lvl != D_USER)
				won[p.difficulty_lvl]++;
		}

		if (is_lost(g))
		{
			draw_game_over(g, r);
			draw_sprite(r, 14, c.x * CELL_SIZE, c.y * CELL_SIZE);

			al_flip_display();

			msg_box(w, "Lost!", "You have lost!");
			reset_game(g);
			first_click = true;

			if(p.difficulty_lvl != D_USER)
				lost[p.difficulty_lvl]++;
		}

		if (is_new_dialog_msg())
		{
			validate(&p); //make params correct

			resize_window(w, p.width * CELL_SIZE, p.height * CELL_SIZE + DOWN_BORDER);
			reset_game_field_params(g, p.width, p.height, p.mines);
			reset_game(g);
			first_click = true;
		}

		if (get_input(w, &c))
		{
			c.x /= CELL_SIZE;
			c.y /= CELL_SIZE;

			if (first_click)
			{
				populate_field(g, c.x, c.y, rand);
				set_field_values(g);
				first_click = false;

				start_time = clock();
			}

			click(g, c.x, c.y, c.left_key);
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_hold_bitmap_drawing(true);

		draw_game_field(g, r);

		int x = field_width(g) * CELL_SIZE;
		int y = field_height(g) * CELL_SIZE + 5;

		draw_sprite(r, 13, x * 0.05, y);
		
		draw_number(r, mines_left(g), x * 0.05 + CELL_SIZE, y);
		draw_number(r, first_click ? 0 : (clock() - start_time) / 1000, x * 0.6, y);
	
		al_hold_bitmap_drawing(false); 
		al_flip_display();
	}
	
	if (first_click)
		remove(SAVE_PATH);

	else
		save_internal_state(g, &p, SAVE_PATH);

	save_stats(STATS_PATH, won, lost);

	destroy_renderer(r);
	destroy_window(w);
	destroy_game(g);

	return 0;
}

