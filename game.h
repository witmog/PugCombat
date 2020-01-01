#pragma once


typedef int Color;
#define RGB32(r, g, b) ((Color)((r << 16) | (g << 8) | b))

// OS to game:
void
platform_fill_rectangle(int x, int y, int width, int height, Color color);

// Game to OS:
void
game_loop(void);
