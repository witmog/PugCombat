#include "chunk.h"

#include "game.h"
#include "blit.h"


// Returns -1 on failure.
int
GetChunkAt(int x, int y)
{
    int i = (x/CHUNK_WIDTH);
    int j = (y/CHUNK_HEIGHT);
    if (i >= WORLD_WIDTH || i < 0 || j >= WORLD_HEIGHT || j < 0)
    {
	return -1;
    }
    return WORLD_WIDTH*j + i;
}

// Returns -1 on failure.
int
GetBlockAt(int x, int y)
{

    int i = x % CHUNK_WIDTH;
    int j = y % CHUNK_HEIGHT;
    if (i >= CHUNK_WIDTH || i < 0 || j >= CHUNK_HEIGHT || j < 0)
    {
	return -1;
    }
    return CHUNK_WIDTH*j + i;
}


static block_t*
CreateChunk(int width, int height)
{
    block_t *chunk = malloc(sizeof(block_t) * width*height);
    for (int j = 0; j < height; ++j)
    {
	for (int i = 0; i < width; ++i)
	{

	    chunk[width*j + i] = NEW_BLOCK(BLOCKTYPE_EMPTY, 0, 0);

	}
    }
    return chunk;
}

static bool
MoveBlock(block_t **world, block_t *current, int x, int y)
{
    int chunk_location;
    int block_location;
    if (((chunk_location = GetChunkAt(x, y)) != -1) &&
	((block_location = GetBlockAt(x, y)) != -1) &&
	(BLOCK_TYPE(world[chunk_location][block_location]) == BLOCKTYPE_EMPTY))
    {
	world[chunk_location][block_location] = NEW_BLOCK(BLOCK_TYPE(*current), 0, 0);
	*current = NEW_BLOCK(BLOCKTYPE_EMPTY, 1, 0);
		
	return true;
    }
    return false;

}

static bool
MarkStill(block_t *current)
{
    *current = NEW_BLOCK(BLOCK_TYPE(*current), BLOCK_UPDATE(*current), 1);
    return true;
}

static void
UpdateChunk(block_t **world, int x, int y)
{

    
    block_t *chunk = world[WORLD_WIDTH*y + x];
    for (int j = CHUNK_HEIGHT-1; j >= 0; --j)
    {
	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
	    block_t *current = chunk + (CHUNK_WIDTH*j + i);

	    if ((BLOCK_TYPE(*current) == BLOCKTYPE_EMPTY) ||
		(BLOCK_UPDATE(*current)) ||
		(BLOCK_STILL(*current)))
	    {
		continue;
	    }

	    int world_x = x*CHUNK_WIDTH + i;
	    int world_y = y*CHUNK_HEIGHT + j;

	    switch (BLOCK_TYPE(*current))
	    {
	    case BLOCKTYPE_LIQUID:
	    {
		MoveBlock(world, current, world_x, world_y+1) ||
		    MoveBlock(world, current, world_x+1, world_y) ||
		    MoveBlock(world, current, world_x-1, world_y);

	    } break;
	    case BLOCKTYPE_SOLID:
	    {
		MoveBlock(world, current, world_x, world_y+1);
	    } break;
	    case BLOCKTYPE_GAS:
	    {
		MoveBlock(world, current, world_x, world_y-1) ||
		    MoveBlock(world, current, world_x+1, world_y) ||
		    MoveBlock(world, current, world_x-1, world_y);
	    } break;
	    case BLOCKTYPE_SAND:
	    {
		MoveBlock(world, current, world_x, world_y+1) ||
		    MoveBlock(world, current, world_x+1, world_y+1) ||
		    MoveBlock(world, current, world_x-1, world_y+1);
	    } break;
	    }
	}
    }

}

static color_t
GetBlockColor(enum BlockType type)
{
    switch (type)
    {
    case BLOCKTYPE_LIQUID:
    {
	return 0xff2323ea;
    } break;
    case BLOCKTYPE_SOLID:
    {
	return 0xff9b7653;
    } break;
    case BLOCKTYPE_GAS:
    {
	return 0xff838383;
    } break;
    case BLOCKTYPE_SAND:
    {
	return 0xffffff99;
    } break;
    default:
    {
	return 0xff000000;
    }
    }
}

static void
RenderChunk(block_t *chunk, int x, int y)
{
    for (int j = 0; j < CHUNK_HEIGHT; ++j)
    {
	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{

	    enum BlockType type = BLOCK_TYPE(chunk[CHUNK_WIDTH*j + i]);
	    if (type != BLOCKTYPE_EMPTY)
	    {
		DrawPoint(i+x, j+y, GetBlockColor(type));
		int still = BLOCK_STILL(chunk[CHUNK_WIDTH*j+i]);

		chunk[CHUNK_WIDTH*j + i] = NEW_BLOCK(type,
						     0,
						     still);
		still = BLOCK_STILL(chunk[CHUNK_WIDTH*j+i]);

	    }
	}
    }
}


block_t **
CreateWorld(int width, int height)
{
    block_t **world = (block_t**)malloc(sizeof(block_t*) * width*height);
    
    for (int j = 0; j < height; ++j)
    {
	for (int i = 0; i < width; ++i)
	{
	    world[width*j + i] = CreateChunk(CHUNK_WIDTH, CHUNK_HEIGHT);
	}
    }
    return world;
}

void
DeleteWorld(block_t **world)
{
    if (world == NULL)
    {
	LOG_WARN("Attempted to delete uninitialized world, aborting.");
	return;
    }
    
    for (int i = 0; i < WORLD_WIDTH*WORLD_HEIGHT; ++i)
    {
	free(world[i]);
    }
    free(world);
}

void
UpdateWorld(block_t **world)
{
    for (int j = 0; j < WORLD_HEIGHT; ++j)
    {
	for (int i = 0; i < WORLD_WIDTH; ++i)
	{
	    UpdateChunk(world, i, j);
	}
    }
}

void
RenderWorld(block_t **world)
{
    for (int j = 0; j < WORLD_HEIGHT; ++j)
    {
	for (int i = 0; i < WORLD_WIDTH; ++i)
	{
	    RenderChunk(world[WORLD_WIDTH*j + i], i*CHUNK_WIDTH, j*CHUNK_HEIGHT);
	}
    }
}


