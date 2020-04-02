#pragma once
#include "language_sugar.h"

typedef uint32_t color_t;
#define GET_ALPHA(color) ((color & (0xFF << 24)) >> 24)
#define BYTES_PER_PIXEL 4

struct Platform
{
	int game_width;
	int game_height;
	int mouse_x;
	int mouse_y;
	bool left_mouse_down;
	bool right_mouse_down;
	bool running;
};

// OS to game:
void
Platform_SetPixel(int x, int y, color_t color);

// Game to OS:
void
InitGame(struct Platform *platform);

void
UpdateGame(void);
