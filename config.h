#pragma once

#define DEFAULT_FIELD_WIDTH 9
#define DEFAULT_FIELD_HEIGHT 9
#define DEFAULT_MINES 10

#define CELL_SIZE 16

#define DOWN_BORDER 48

#define DEFAULT_WINDOW_WIDTH (DEFAULT_FIELD_WIDTH * CELL_SIZE)
#define DEFAULT_WINDOW_HEIGHT (DEFAULT_FIELD_HEIGHT * CELL_SIZE + DOWN_BORDER)

#define MAX_FIELD_WIDTH 30
#define MAX_FIELD_HEIGHT 24
#define MAX_MINES 668

#define MIN_FIELD_WIDTH 9
#define MIN_FIELD_HEIGHT 9
#define MIN_MINES 1

#define SAVE_PATH "saper_save.dat"
#define STATS_PATH "stats.dat"

typedef enum FIELD_SIZE_ { F_SMALL, F_MEDIUM, F_BIG, F_USER } FIELD_SIZE;
typedef enum DIFF_LVL_ {
	D_VERY_EASY, D_EASY, D_NORMAL, D_MEDIUM_HARD,
	D_HARD, D_VERY_HARD, D_SUPER_HARD, D_ULTRA_HARD, D_USER
} DIFF_LVL;

typedef struct 
{
	char text[32];
	int size;
} bid;

static const bid field_lookup[] =
{
	{ "Small (9x9)", 9 },
	{ "Medium (16x16)", 16 },
	{ "Big (24x24)", 24 }
};

static const bid diff_lookup[] =
{
	{ "Very easy (5 mines)", 5 },
	{ "Easy (10 mines)", 10 },
	{ "Medium (15 mines)", 15 },
	{ "Medicore (20 mines)", 20 },
	{ "Hard (25 mines)", 25 },
	{ "Very hard (30 mines)", 30 },
	{ "Super hard (40 mines)", 40 },
	{ "Ultra hard (50 mines)", 50 }
};

