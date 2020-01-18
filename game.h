#pragma once
#include <common/language_sugar.h>

typedef u32 Color;
#define RGBA32(r, g, b, a) ((Color)((a << 24) | (r << 16) | (g << 8) | b))
#define GET_ALPHA(color) ((color & (0xFF << 24)) >> 24)
#define BYTES_PER_PIXEL 4

typedef struct Platform Platform;
struct Platform
{
	i32 screen_width;
	i32 screen_height;
	i32 mouse_x;
	i32 mouse_y;
	b8 left_mouse_down;
	b8 right_mouse_down;
	b8 running;
};

// OS to game:
void
platform_set_pixel(i32 x, i32 y, Color color);

// Game to OS:
void
game_init(Platform *platform);

void
game_loop(void);
