#pragma once

typedef int Color;
#define RGB32(r, g, b) ((Color)((r << 16) | (g << 8) | b))
#define BYTES_PER_PIXEL 4

// OS to game:
void
platform_set_pixel(int x, int y, Color color);

// Game to OS:
void
game_init(void);

void
game_loop(void);
