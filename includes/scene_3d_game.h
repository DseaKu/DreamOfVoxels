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
#define N_CHUNKS_X 2
#define N_CHUNKS_Y 2
#define CHUNKS_IN_TOTAL N_CHUNKS_Y *N_CHUNKS_X
typedef u64 Voxel;
typedef struct Chunk {
  u8 y_offset;
  u8 x_offset;
  Voxel *p_voxel_data;
  bool is_dirty;
} Chunk;

int Scene3DGame();
void Draw3DDebugInformation(int screen_width, int screen_height);
void Draw2DDebugInformation(int screen_width, int screen_height);

Chunk *InitChunks();
u8 DestroyAllChunks(Chunk *chunk_data);
#endif // SCENE_3D_GAME_H
