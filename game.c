#include <game.h>
#include <sprite.h>
#include <common/language_sugar.h>
#include <stdlib.h>

global Platform *platform;
global Sprite blue_square;
Sprite terrain;
i32 terrain_x = 0;
i32 terrain_y = 300;

internal void
fill_rectangle(i32 x, i32 y, i32 width, i32 height, Color color)
{
	for (i32 j = y; j < height+y; ++j)
	{
		for (i32 i = x; i < width+x; ++i)
		{
			platform_set_pixel(i, j, color);	
		}
	}
}


void
game_init(Platform *platform_)
{
	platform = platform_;
	blue_square = sprite_from_file("blue.sprite");

	// Todo: load terrain from disk
	{
		terrain.width = 800;
		terrain.height = 600;
		u32 area = terrain.width*terrain.height;
		terrain.data = malloc(sizeof(Color)*area);
		for (u32 i = 0; i < area; ++i)
		{
			terrain.data[i] = RGBA32(84, 255, 84, 255);
		}
	}
}

void
game_loop(void)
{
	fill_rectangle(0, 0, platform->screen_width, platform->screen_height, RGBA32(23, 2, 251, 255));

	if (platform->left_mouse_down)
	{
		fill_circle_in_sprite(platform->mouse_x-terrain_x,
				      platform->mouse_y-terrain_y, 
				      &terrain, 32, 0);
	}
	sprite_draw(terrain_x, terrain_y, &terrain);
}
