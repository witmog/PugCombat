#include <game.h>
#include <sprite.h>
#include <common/language_sugar.h>
#include <stdlib.h>

global Platform *platform;
global Sprite blue_square;
Sprite terrain;
i32 terrain_x = 0;
i32 terrain_y = 300;

#define MAX_BOMBS 10
typedef struct Bombs Bombs;
struct Bombs
{
	int x[MAX_BOMBS];
	int y[MAX_BOMBS];
	Sprite sprite[MAX_BOMBS];
	b32 active[MAX_BOMBS];
};
Bombs bombs;

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


internal void
init_bombs(void)
{
	for (i32 i = 0; i < MAX_BOMBS; ++i)
	{
		bombs.x[i] = 25;
		bombs.y[i] = 0;
		bombs.sprite[i].width = 25;
		bombs.sprite[i].height = 25;
		bombs.sprite[i].data = malloc(25*25 * sizeof(Color));
		bombs.active[i] = true;
		for (i32 j = 0; j < 25; ++j)
		{
			for (i32 k = 0; k < 25; ++k)
			{
				int alpha = 255;
				if (k < 10 || k > 15)
				{
					alpha = 0;
				}
				bombs.sprite[i].data[bombs.sprite[i].width*j+k] = RGBA32(0, 0, 0, alpha);
			}
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

	init_bombs();
}

void
game_loop(void)
{
	fill_rectangle(0, 0, platform->screen_width, platform->screen_height, RGBA32(23, 2, 251, 255));

	if (platform->left_mouse_down)
	{
	}
	sprite_draw(terrain_x, terrain_y, &terrain);

	for (i32 i = 0; i < MAX_BOMBS; ++i)
	{
		
		if ((i == 0 && bombs.active[i]) || (!bombs.active[i-1] && bombs.active[i]))
		{
			bombs.y[i] += 1;
			if (sprite_collide_with_sprite(bombs.x[i], bombs.y[i], &bombs.sprite[i],
							 terrain_x, terrain_y, &terrain))
			{
				fill_circle_in_sprite(bombs.x[i]-terrain_x,
						      bombs.y[i]-terrain_y-(bombs.sprite[i].height/2), 
						      &terrain, 64, 0);
				bombs.active[i] = false;
			}
			sprite_draw(bombs.x[i], bombs.y[i], &bombs.sprite[i]); 
		}
	}
}
