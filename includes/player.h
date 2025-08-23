#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "scene_voxel_game.h"
//----------------------------------------------------------------------------------
// Movement constants
//----------------------------------------------------------------------------------
#define GRAVITY 32.0f
#define MAX_SPEED 10.0f
#define CROUCH_SPEED 0.8f
#define JUMP_FORCE 11.0f
#define MAX_ACCEL 1010.0f
#define CROUCH_HEIGHT 0.0f
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f
// Grounded drag
#define FRICTION .97f
// Increasing air drag, increases strafing speed
#define AIR_DRAG 0.97f
// Responsiveness for turning movement direction to looked direction
#define CONTROL 15.0f

#define PLAYER_COLLISION_SHAPE_X VOXEL_SIZE
#define PLAYER_COLLISION_SHAPE_Z VOXEL_SIZE
#define PLAYER_COLLISION_SHAPE_Y VOXEL_SIZE * 2
#define NORMALIZE_INPUT 0

void UpdateCameraAngle(Player *player);
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold, Chunk current_chunk);

Vector3 UpdateBodyPosition(Vector3 velocity, float delta);
Player InitPlayer(void);
void UpdatePlayer(Player *player, Chunk *chunk_data);
s16Vector2D GetXZCurrentChunk(Player *player);
u32 GetIndexCurrentChunk(Player *player);

#endif // PLAYER_H
