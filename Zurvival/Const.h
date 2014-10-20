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