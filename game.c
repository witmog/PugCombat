#include <game.h>
#include <sprite.h>

static Platform *platform;
static Sprite blue_square;

static void
fill_rectangle(int x, int y, int width, int height, Color color)
{
	for (int j = y; j < height+y; ++j)
	{
		for (int i = x; i < width+x; ++i)
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
	sprite_draw(0, 25, &blue_square);
	if (platform->right_mouse_down)
	fill_rectangle(platform->mouse_x, platform->mouse_y, 32, 32, RGB32(255, 255, 0));
}
