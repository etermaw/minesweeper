#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "saves.h"

void save_internal_state(game* g, game_params* p, const char* filename)
{
	FILE* save = fopen(SAVE_PATH, "wb");

	fwrite(p, sizeof(game_params), 1, save);
	save_gamestate(g, save);

	fclose(save);
}

bool restore_internal_state(game* g, game_params* p, const char* filename)
{
	FILE* save = fopen(filename, "rb");

	if (save)
	{
		fread(p, sizeof(game_params), 1, save);
		restore_gamestate(g, save);

		fclose(save);

		return true;
	}

	return false;
}

void load_stats(const char* filename, size_t* wins_arr, size_t* lost_arr)
{
	FILE* s = fopen(filename, "rb");

	if (s)
	{
		fread(wins_arr, sizeof(size_t), 8, s);
		fread(lost_arr, sizeof(size_t), 8, s);

		fclose(s);
	}
}

void save_stats(const char* filename, size_t* wins_arr, size_t* lost_arr)
{
	FILE* s = fopen(filename, "wb");

	fwrite(wins_arr, sizeof(size_t), 8, s);
	fwrite(lost_arr, sizeof(size_t), 8, s);

	fclose(s);
}