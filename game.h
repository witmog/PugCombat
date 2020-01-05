#pragma once


typedef int Color;
#define RGB32(r, g, b) ((Color)((r << 16) | (g << 8) | b))

// OS to game:
void
platform_set_pixel(int x, int y, Color color);

// Game to OS:
void
game_loop(void);
