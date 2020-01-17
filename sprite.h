#pragma once
#include <game.h>
#include <common/language_sugar.h>

typedef struct Sprite Sprite;
struct Sprite
{
	u32 width;
	u32 height;
	Color *data;
};

void
sprite_draw(i32 x, i32 y, Sprite *sprite);

void
fill_circle_in_sprite(i32 x, i32 y,
		      Sprite *sprite,
		      u32 radius,
		      Color color);

void
sprite_to_disk(char *filepath, Sprite *sprite);

Sprite
sprite_from_file(char *filepath);
