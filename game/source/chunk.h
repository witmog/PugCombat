#pragma once

#define WORLD_WIDTH 4 
#define WORLD_HEIGHT 3
#define CHUNK_WIDTH 64
#define CHUNK_HEIGHT 64

/*
    Memory Layout
/==============================\
|      block_t                 |
|      8 bits                  |
|------------------------------|
| update | static | block type |
| 1 bit  | 1 bit  | 6 bits     |
\==============================/

*/

// NOTE: 6 bits = maximum of 63 block types
enum BlockType
{
	BLOCKTYPE_EMPTY = 0,
	BLOCKTYPE_LIQUID,
	BLOCKTYPE_SOLID,
	BLOCKTYPE_GAS,
	BLOCKTYPE_SAND
};


typedef uint8_t block_t;

// TODO: also have static bit for blocks that won't move for more than a frame.
#define NEW_BLOCK(type, update, still) ((((0xFF >> 7) & update) << 7) |	\
					(((0xFF >> 7) & still) << 6)  | \
					((0xFF >> 2) & type))
#define BLOCK_TYPE(b) ((0xFF >> 2) & b)
#define BLOCK_UPDATE(b) (((0xFF << 7) & b) >> 7)
#define BLOCK_STILL(b) (((0x1 << 6) & b) >> 6)

block_t **
CreateWorld(int width, int height);

void
DeleteWorld(block_t **world);

void
UpdateWorld(block_t **world);

void
RenderWorld(block_t **world);

int
GetChunkAt(int x, int y);

int
GetBlockAt(int x, int y);
