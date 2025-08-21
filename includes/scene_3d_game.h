#include <raylib.h>
#ifndef SCENE_3D_GAME_H
#include "std_includes.h"
#define SCENE_3D_GAME_H

#define MAX_SCREEN_WIDTH 1400
#define MAX_SCREEN_HEIGHT 900
#define CURSOR_RADIUS 2

#define TARGET_FPS 3000
#define BLOCK_SIZE 1.0f
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2.0f)
#define MAX_PLAYER_RANGE 4.0f
#define N_CHUNKS_X 4
#define N_CHUNKS_Z 4
#define CHUNKS_IN_TOTAL N_CHUNKS_Z *N_CHUNKS_X
typedef u64 Voxel;
typedef struct SnVector2D {
  s64 x_offset;
  s64 z_offset;
} SnVector2D;

typedef struct Chunk {
  bool is_dirty;
  SnVector2D position;
  Voxel *p_voxel_data;
  Mesh chunk_mesh;
} Chunk;

/*
 *
 *            z pos
 *           |
 *           |
 *  x=1,z=1  |   x=-1,z=1
 *           |
 *   ------------------
 * x pos     |  x neg    e
 *           |
 *  x=1,z=-1 |   x=-1,z=-1
 *           |
 *           |
 *            z neg
 *
 * */

int Scene3DGame();
void Draw3DDebugInformation(int screen_width, int screen_height);
void Draw2DDebugInformation(int screen_width, int screen_height);

Chunk *InitChunks();
void FreeAllChunkData(Chunk *chunk_data);
#endif // SCENE_3D_GAME_H
