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
#define DRAWN_CHUNKS 4

#define CHUNK_SIZE 32

#define TILE_FOR_HEIGHT 10

// light definitions
// establishes a factor of reduction nfor faster computation times
#define LIGHT_REDUCTION 8
// begin alpha [0-LIGHT_FINAL_ALPHA]
#define LIGHT_BEGIN_ALPHA 50
// final alpha [LIGHT_BEGIN_ALPHA-255]
#define LIGHT_FINAL_ALPHA 255
// light view range [0-LIGHT_VIEW_MAX_RANGE] 1 meaning height/2
#define LIGHT_VIEW_MIN_RANGE 0.2
// light view range [LIGHT_VIEW_MIN_RANGE-inf] 1 meaning height/2
#define LIGHT_VIEW_MAX_RANGE 1
// light color [0-0xffffff] BGR
#define LIGHT_BASE_COLOR 0
// angle in radians for the view cone
#define LIGHT_VIEW_ANGLE 1.05