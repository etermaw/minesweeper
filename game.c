#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "game.h"
#include "config.h"
#include "vector.h"
#include "renderer.h"

#define SET_BIT(holder,bit_num) ((holder) |= (1 << (bit_num)))
#define UNSET_BIT(holder,bit_num) ((holder) &= ~(1 << (bit_num)))
#define GET_BIT(holder,bit_num) (((holder) & (1 << (bit_num))) >> (bit_num))
#define APPLY_MASK(holder,mask) ((holder) & (mask))

enum FIELD_FLAGS { F_MINED, F_VISIBLE, F_FLAG_MINE, F_FLAG_UNKNOWN };

typedef struct
{
	char flags;
	char value;
} field;

struct game
{
	field* game_field;

	unsigned mines_count,
			 mines_flag_count,
			 field_width,
			 field_height;

	bool alive;
};

//show_empty_fields is private function!
//struct point is local, because it`s needed only in this function
void show_empty_fields(game* obj, int sx, int sy)
{
	typedef struct
	{
		int x, y;
	} point;

	vector* fields_to_show = create_vector(sizeof(point));

	point* tmp = (point*)push_back(fields_to_show);

	tmp->x = sx;
	tmp->y = sy;

	while (size(fields_to_show) != 0)
	{
		point* p = (point*)pop_back(fields_to_show);

		int x = p->x;
		int y = p->y;

		if (x < 0 || y < 0 || x >= obj->field_width || y >= obj->field_height)
			continue;

		field* f = obj->game_field + (y * obj->field_width + x);

		// 0x7 == 0111, we just check if field is not mined,visible or flagged
		if (!APPLY_MASK(f->flags, 0x7))
		{
			SET_BIT(f->flags, F_VISIBLE);

			if (f->value == 0)
			{
				for (int i = -1; i < 2; ++i)
				{
					for (int j = -1; j < 2; ++j)
					{
						tmp = (point*)push_back(fields_to_show);

						tmp->x = x + i;
						tmp->y = y + j;
					}
				}
			}
		}
	}

	destroy_vector(fields_to_show);
}

game* create_game()
{
	game* tmp = (game*)malloc(sizeof(game));

	tmp->game_field = (field*)malloc(sizeof(field) * MAX_FIELD_WIDTH * MAX_FIELD_HEIGHT);
	tmp->mines_count = DEFAULT_MINES;
	tmp->field_width = DEFAULT_FIELD_WIDTH;
	tmp->field_height = DEFAULT_FIELD_HEIGHT;
	tmp->mines_flag_count = 0;
	tmp->alive = true;

	memset(tmp->game_field, 0, sizeof(field) * tmp->field_height * tmp->field_width);

	return tmp;
}

void destroy_game(game* obj)
{
	free(obj->game_field);
	free(obj);
}

#define clamp(a,b,c) ((a) < (b) ? (b) : ((a) > (c) ? (c) : (a)))

void reset_game_field_params(game* obj, unsigned width, unsigned height, unsigned mines)
{
	obj->field_width = clamp(width, MIN_FIELD_WIDTH, MAX_FIELD_WIDTH);
	obj->field_height = clamp(height, MIN_FIELD_HEIGHT, MAX_FIELD_HEIGHT);
	obj->mines_count = clamp(mines, MIN_MINES, obj->field_width * obj->field_height * 0.95);
}

unsigned mines_left(game* obj)
{
	int ret = obj->mines_count - obj->mines_flag_count;

	return clamp(ret, 0, obj->mines_count);
}

#undef clamp(a,b,c)

void populate_field(game* obj, int sx, int sy, int(*rng_func)())
{
	int i = 0;

	while (i < obj->mines_count)
	{
		int x = rng_func() % obj->field_width;
		int y = rng_func() % obj->field_height;

		field* f = obj->game_field + (y * obj->field_width + x);

		if (!GET_BIT(f->flags, F_MINED) && (x != sx || y != sy))
		{
			SET_BIT(f->flags, F_MINED);
			++i;
		}
	}
}

void click(game* obj, int x, int y, bool left_key)
{
	if (x >= obj->field_width || y >= obj->field_height)
		return;

	field* f = obj->game_field + (y * obj->field_width + x);

	if (left_key)
	{
		if (GET_BIT(f->flags, F_MINED) && !GET_BIT(f->flags, F_FLAG_MINE))
			obj->alive = false;

		else
			show_empty_fields(obj, x, y);
	}

	else if(!GET_BIT(f->flags, F_VISIBLE))
	{
		if (GET_BIT(f->flags, F_FLAG_UNKNOWN))
			UNSET_BIT(f->flags, F_FLAG_UNKNOWN);

		else if (!GET_BIT(f->flags, F_FLAG_MINE))
		{
			SET_BIT(f->flags, F_FLAG_MINE);
			obj->mines_flag_count++;
		}

		else
		{
			UNSET_BIT(f->flags, F_FLAG_MINE);
			SET_BIT(f->flags, F_FLAG_UNKNOWN);
			obj->mines_flag_count--;
		}
	}
}

void set_field_values(game* obj)
{
	unsigned w = obj->field_width,
			 h = obj->field_height;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			if (GET_BIT(obj->game_field[i * w + j].flags, F_MINED))
			{
				for (int k = -1; k < 2; ++k)
				{
					for (int l = -1; l < 2; ++l)
					{
						if ((i + k < 0) || (j + l < 0) || (i + k >= h) || (j + l >= w))
							continue;

						obj->game_field[(i + k) * w + (j + l)].value++;
					}
				}
			}
		}
	}
}

void reset_game(game* obj)
{
	memset(obj->game_field, 0, sizeof(field) * obj->field_width * obj->field_height);
	obj->alive = true;
	obj->mines_flag_count = 0;
}

unsigned field_width(game* obj)
{
	return obj->field_width;
}

unsigned field_height(game* obj)
{
	return obj->field_height;
}

bool is_won(game* obj)
{
	size_t size = obj->field_width * obj->field_height;

	for (int i = 0; i < size; ++i)
		if (!APPLY_MASK(obj->game_field[i].flags, 0x3)) //check if any non-mined field is visible
			return false;

	return true;
}

bool is_lost(game* obj)
{
	return !obj->alive;
}

void save_gamestate(game* obj, FILE* save_file)
{
	fprintf_s(save_file, "%u %u %u %u ", obj->field_width, obj->field_height, obj->mines_count, obj->mines_flag_count);
	fwrite(obj->game_field, sizeof(field), obj->field_height*obj->field_width, save_file);
}

void restore_gamestate(game* obj, FILE* save_file)
{
	fscanf_s(save_file, "%u %u %u %u ", &obj->field_width, &obj->field_height, &obj->mines_count, &obj->mines_flag_count);
	fread(obj->game_field, sizeof(field), obj->field_height*obj->field_width, save_file);
}

void draw_game_field(game* obj, renderer* rend)
{
	const size_t w = obj->field_width;
	const size_t h = obj->field_height;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			char f = obj->game_field[i * w + j].flags;
			char sid = obj->game_field[i * w + j].value;

			//F_FLAG_MINE and F_FLAG_UNKNOWN are mutual exlusive
			//sprite id`s: 9 - not clicked, 10 - flagged as mine, 11 - flagged as '?'
			if (!GET_BIT(f, F_VISIBLE))
				sid = 9 + GET_BIT(f, F_FLAG_MINE) + (GET_BIT(f, F_FLAG_UNKNOWN) << 1);

			draw_sprite(rend, sid, j * CELL_SIZE, i * CELL_SIZE);
		}
	}
}

void draw_game_over(game* obj, renderer* rend)
{
	const size_t w = obj->field_width;
	const size_t h = obj->field_height;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			char f = obj->game_field[i * w + j].flags;
			char sid = obj->game_field[i * w + j].value;

			if (GET_BIT(f, F_MINED))
				sid = GET_BIT(f, F_FLAG_MINE) ? 10 : 13;

			else if (!GET_BIT(f, F_VISIBLE))
				sid = GET_BIT(f, F_FLAG_MINE) ? 15 : 9;

			draw_sprite(rend, sid, j * CELL_SIZE, i * CELL_SIZE);
		}
	}
}

