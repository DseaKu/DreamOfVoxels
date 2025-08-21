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

#define NUMBER_OF_CHUNK_RINGS 1
typedef u64 Voxel;
typedef struct Chunk {
  s16 y_offset;
  s16 x_offset;
  Voxel *p_voxel_data;
  bool is_dirty;
} Chunk;

int Scene3DGame();
void Draw3DDebugInformation(int screen_width, int screen_height);
void Draw2DDebugInformation(int screen_width, int screen_height);

Chunk *InitChunks(u16 n_chunks);
u16 CalcNumChuncks(u8 num);
u8 DestroyAllChunks(Chunk *chunk_data);
#endif // SCENE_3D_GAME_H
