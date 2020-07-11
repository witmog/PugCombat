#include "game.h"
#include "language_sugar.h"
#include "vector.h"
#include "blit.h"
#include "logger.h"
#include "chunk.h"

#include "blit.c"
#include "logger.c"
#include "chunk.c"

struct Platform *platform;

block_t **world;

internal void
print_vector(vector(int) v)
{
	log_debug("vector_count = %d", vector_count(v));
	for (int i = 0; i < vector_count(v); ++i)
	{
		log_debug("v[%d] = %d", i, v[i]);
	}
}
void
InitGame(struct Platform *_platform)
{
	log_set_minimum_level(LOGLEVEL_DEBUG);	
    platform = _platform;
    world = CreateWorld(WORLD_WIDTH, WORLD_HEIGHT);

    // Create dynamic array
    vector(int) list = VECTOR_EMPTY;
    vector_reserve(list, 3);

    // Push   = add to end of array
    // Pop    = remove from end of array, returns the element that was removed
    // Insert = add element to array at a specific index
    // Remove = remove element from array at a specific index
    // FindRemove = search for a specific element and remove it if it exists
    vector_push(list, 23);
    vector_push(list, 4);
    vector_insert(list, 1, 69);
    vector_remove(list, 2);
    print_vector(list);
    vector_pop(list);
    vector_push(list, 43);
    vector_find_remove(list, 23);
    vector_free(list);


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
	    
	world[chunk_location][tile_location] = NEW_BLOCK(BLOCKTYPE_WATER, 0);
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
		    
		    world[chunk_location][tile_location] = NEW_BLOCK(BLOCKTYPE_SOLID, 1);
		}

	    }
	}
    }

    UpdateWorld(world);
    RenderWorld(world);

}
