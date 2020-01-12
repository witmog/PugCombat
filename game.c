#include <game.h>
#include <sprite.h>
#include <common/language_sugar.h>

global Platform *platform;
global Sprite blue_square;

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
}

void
game_loop(void)
{
	fill_rectangle(0, 0, platform->screen_width, platform->screen_height, RGB32(144, 23, 86));
	sprite_draw(platform->mouse_x, platform->mouse_y, &blue_square);
	if (platform->right_mouse_down)
	fill_rectangle(platform->mouse_x, platform->mouse_y, 32, 32, RGB32(255, 255, 0));
}
