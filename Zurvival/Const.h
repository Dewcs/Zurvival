#pragma once

enum verbose_t {
	VERBOSE_NONE = 0,
	VERBOSE_BASIC = 1,
	VERBOSE_USER_INPUT = 2,
	VERBOSE_DATA_CREATION = 4,
	VERBOSE_ERRORS = 8,
	VERBOSE_FRAME_DRAW = 16,
	VERBOSE_LOAD = 32,
	VERBOSE_BRAIN = 64,
	VERBOSE_TRAIN = 128,
	VERBOSE_ALL = 0xFFFFFFFF,
};

#define VERBOSE_LEVEL VERBOSE_ALL&~(VERBOSE_FRAME_DRAW|VERBOSE_BRAIN)

enum page_t {
	LOADING,
	MAINMENU,
	DEATHPIT,
	OPTIONS,
	GAME, 
	GAMEOVER
};

enum order_t {
	ORDER_NONE,
	ORDER_CHANGE_PAGE,
};

enum align_t {
	ALIGN_RIGHT,
	ALIGN_LEFT,
	ALIGN_CENTER
};

enum move_t {
	MOVE_TOP = 1,
	MOVE_RIGHT = 2,
	MOVE_BOT = 4,
	MOVE_LEFT = 8
};
enum tile_t {
	GRASS,
	WATER
};

enum actor_t {
	ACTOR_ZOMBIE,
	ACTOR_HUMAN,
};

// x and y offsets for chunk neighbors
const int xval[] = { 0, 1, 0, -1 };
const int yval[] = { 1, 0, -1, 0 };

// should calculate but atm is fine
#define DRAWN_CHUNKS 4

// how many chunks can have the map in x and y coordinates power of 2 please

#define MAX_CHUNK 256

// wisth and height of map chunks
#define CHUNK_SIZE 32

// how many tiles on screen heightwise
#define TILE_FOR_HEIGHT 10

// max amount of zombies on the map
#define ZOMBIE_AMOUNT 100

// light definitions
// establishes a factor of reduction for faster computation times
#define LIGHT_REDUCTION 8
// light reduction factor
#define LIGHT_DISTANCE 0.7
// begin alpha [0-LIGHT_FINAL_ALPHA]
#define LIGHT_BEGIN_ALPHA 50
// final alpha [LIGHT_BEGIN_ALPHA-255]
#define LIGHT_FINAL_ALPHA 255
// light color [0-0xffffff] BGR
#define LIGHT_BASE_COLOR 0

// DEAHT PIT
#define DP_ZOMBIE_AMOUNT 80// number of zombies
#define DP_HUMAN_AMOUNT 20 // number of humans
#define DP_RATIO 4 // size divider 
#define DP_TIME_SPEED 2 // time multiplier

#define DP_RANDOM_ZOMBIE_CHANCES 20 // [0-99] chances to spawn a random zombie 
#define DP_RANDOM_HUMAN_CHANCES 20 // [0-99] chances to spawn a random human

#define MAX_ZOMBIE_STORAGE 200 // how many zombies ia to store
#define MAX_HUMAN_STORAGE 200 // how many human ia to store

#define ITEMS_PER_SQM 0.005 // how many items per square meter