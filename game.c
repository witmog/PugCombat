#include <game.h>
#include <math.h>

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
game_init(void)
{
	
}

void
game_loop(void)
{
	fill_rectangle(0, 0, 25, 200, RGB32(144, 23, 86));
}
