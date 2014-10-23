#pragma once

enum page_t {
	LOADING,
	MAINMENU,
	DEATHPIT,
	OPTIONS,
	GAME
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

// should calculate but atm is fine
#define DRAWN_CHUNKS 4

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