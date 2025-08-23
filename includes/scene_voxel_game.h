#include <raylib.h>
#ifndef SCENE_3D_GAME_H
#include "std_includes.h"
#define SCENE_3D_GAME_H

//----------------------------------------------------------------------------------
// Defines Window
//----------------------------------------------------------------------------------
#define MAX_SCREEN_WIDTH 800
#define MAX_SCREEN_HEIGHT 600
#define CURSOR_RADIUS 2
#define TARGET_FPS 3000
#define MAX_PLAYER_RANGE 4.0f

//----------------------------------------------------------------------------------
// Defines Chunk
//----------------------------------------------------------------------------------
#define CHUNK_SIZE 16
#define N_CHUNKS_X 2
#define N_CHUNKS_Z 2
#define CHUNKS_IN_TOTAL N_CHUNKS_Z *N_CHUNKS_X
//----------------------------------------------------------------------------------
// Defines  Voxel
//----------------------------------------------------------------------------------
#define VOXEL_SIZE 1.0f
#define HALF_VOXEL_SIZE 0.5f / 2.0f

typedef u64 Voxel;

typedef struct Chunk {
  bool is_mesh_dirty;
  s16Vector2D position;
  Voxel *p_voxel_data;
  Mesh chunk_mesh;
} Chunk;

//----------------------------------------------------------------------------------
// Defines Player
//----------------------------------------------------------------------------------
typedef struct {
  bool isGrounded;
  float headTimer;
  float walkLerp;
  float headLerp;
  Vector2 lean;
  Vector2 sensitivity;
  Vector2 lookRotation;
  Vector3 position;
  s64Vector3D voxel_position;

  Vector3 velocity;
  Vector3 dir;
  BoundingBox collision_shape;
} Body;

typedef struct Player {
  Camera camera;
  Body body;
} Player;

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
void Draw2DDebugInformation(int screen_width, int screen_height,
                            Chunk *chunk_data, Player *player);

Chunk *InitChunks();
void FreeAllChunkData(Chunk *chunk_data);
#endif // SCENE_3D_GAME_H
