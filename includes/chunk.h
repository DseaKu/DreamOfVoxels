#ifndef CHUNK_H
#define CHUNK_H

#include "scene_voxel_game.h"
#include "std_includes.h"
#include "voxel.h"
#include <math.h>
Chunk *InitChunks();
void FreeAllChunkData(Chunk *chunk_data);
u32 GetIndexCurrentChunk(Player *player);
u64 GetIndexXZYChunk(Chunk *chunk_data, s64Vector3D);

s16Vector2D GetXZCurrentChunk(Player *player);

#endif // !CHUNK_H
