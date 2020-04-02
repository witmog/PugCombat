#include "game.h"
#include "language_sugar.h"
#include "blit.h"
#include "logger.h"
#include "chunk.h"

#include "blit.c"
#include "logger.c"
#include "chunk.c"

struct Platform *platform;

block_t **world;

void
InitGame(struct Platform *_platform)
{
    platform = _platform;
    world = CreateWorld(WORLD_WIDTH, WORLD_HEIGHT);
    //  block_t sample = NEW_BLOCK(BLOCKTYPE_SOLID, 1, 1);
//    LOG_TRACE("The block still: %d", BLOCK_STILL(sample));
}

void
UpdateGame(void)
{
    DrawRectangle(0, 0, platform->game_width, platform->game_height, 0xff8989ff);

    if ((platform->left_mouse_down) &&
	(platform->mouse_x >= 0  && platform->mouse_y >= 0) &&
	(platform->mouse_x < CHUNK_WIDTH*WORLD_WIDTH) &&
	(platform->mouse_y < CHUNK_HEIGHT*WORLD_HEIGHT))
    {
	int chunk_location = GetChunkAt(platform->mouse_x, platform->mouse_y);
	int tile_location = GetBlockAt(platform->mouse_x, platform->mouse_y);
	    
	world[chunk_location][tile_location] = NEW_BLOCK(BLOCKTYPE_SAND, 0, 0);
    }

    if ((platform->right_mouse_down) &&
	(platform->mouse_x >= 0  && platform->mouse_y >= 0) &&
	(platform->mouse_x < CHUNK_WIDTH*WORLD_WIDTH) &&
	(platform->mouse_y < CHUNK_HEIGHT*WORLD_HEIGHT))
    {
	int point_x = platform->mouse_x;
	int point_y = platform->mouse_y;
	for (int y = 0; y < 2; ++y)
	{
	    for (int x = 0; x < 2; ++x)
	    {
		
		int chunk_location = GetChunkAt(point_x+x, point_y+y);
		int tile_location = GetBlockAt(point_x+x, point_y+y);
		if (chunk_location != -1 && tile_location != -1)
		{
		    
		    world[chunk_location][tile_location] = NEW_BLOCK(BLOCKTYPE_SOLID, 1, 1);
		}

	    }
	}
    }

    UpdateWorld(world);
    RenderWorld(world);

}
