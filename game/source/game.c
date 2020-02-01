#include "game.h"
#include "sprite.h"
#include "language_sugar.h"

#include "sprite.c"

global Platform *platform;
Sprite terrain;
i32 terrain_x = 0;
i32 terrain_y = 300;

#define MAX_BOMBS 1
typedef struct Bombs Bombs;
struct Bombs
{
	float x[MAX_BOMBS];
	float y[MAX_BOMBS];
	Sprite sprite[MAX_BOMBS];
	b32 active[MAX_BOMBS];
	float vel_y[MAX_BOMBS];
	float direction[MAX_BOMBS];
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
		bombs.x[i] = 300;
		bombs.y[i] = 300;
		bombs.sprite[i].width = 8;
		bombs.sprite[i].height = 8;
		bombs.sprite[i].data = malloc(8*8 * sizeof(Color));
		bombs.active[i] = false;
		bombs.vel_y[i] = 0;
		for (i32 j = 0; j < 8; ++j)
		{
			for (i32 k = 0; k < 8; ++k)
			{
				bombs.sprite[i].data[bombs.sprite[i].width*j+k] = RGBA32(95, 95, 95, 255);
			}
		}
	}
}
void
game_init(Platform *platform_)
{
	platform = platform_;

	// TODO: load terrain from disk
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


	sprite_draw(terrain_x, terrain_y, &terrain);

	float player_x = 300;
	float player_y = 300-16;
	fill_rectangle(player_x-8, player_y-8, 16, 16, RGBA32(0, 255, 0, 255));

	int radius = 32;
	float distance = sqrt(pow(platform->mouse_x-player_x, 2) +
			      pow(platform->mouse_y-player_y, 2));

	int x = (platform->mouse_x-player_x) *
		(radius/distance) + player_x;
	int y = (platform->mouse_y-player_y) *
		(radius/distance) + player_y;

	fill_rectangle(x-4, y-4, 8, 8, RGBA32(255, 255, 255, 255));

	for (i32 i = 0; i < MAX_BOMBS; ++i)
	{
		if (platform->left_mouse_down && bombs.active[i] == false)
		{
			bombs.direction[i] = atan2(platform->mouse_y-player_y, platform->mouse_x-player_x);
			bombs.active[i] = true;
			bombs.x[i] = player_x;
			bombs.y[i] = player_y;
			bombs.vel_y[i] = 0;
		}
		
		if (bombs.active[i])
		{
			bombs.x[i] += cos(bombs.direction[i])*2.0f;
			bombs.y[i] += sin(bombs.direction[i])*2.0f + bombs.vel_y[i];
			bombs.vel_y[i] = clamp(bombs.vel_y[i] + 0.005f, 0, 3);

			if (sprite_collide_with_sprite(bombs.x[i], bombs.y[i], &bombs.sprite[i],
							 terrain_x, terrain_y, &terrain))
			{
				fill_circle_in_sprite(bombs.x[i]-terrain_x,
						      bombs.y[i]-terrain_y-(bombs.sprite[i].height/2), 
						      &terrain, 64, 0);
				bombs.active[i] = false;
			}
			sprite_draw(bombs.x[i], bombs.y[i], &bombs.sprite[i]); 

			// TODO: deactive after specific travel distance?
			// This is a temporary fail-safe, in case
			// the bullet never hits any ground.
			if (bombs.y[i] > terrain_y+terrain.height)
			{
				bombs.active[i] = false;
			}
		}
	}

}
