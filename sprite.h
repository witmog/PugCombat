#pragma once
#include <game.h>

typedef struct
{
	unsigned int width;
	unsigned int height;
	Color *data;
} Sprite;

void
sprite_draw(int x, int y, Sprite *sprite);

void
sprite_to_disk(char *filepath, Sprite *sprite);

Sprite
sprite_from_file(char *filepath);
