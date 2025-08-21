#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "scene_3d_game.h"
//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Movement constants
#define GRAVITY 32.0f
#define MAX_SPEED 40.0f
#define CROUCH_SPEED 5.0f
#define JUMP_FORCE 11.0f
#define MAX_ACCEL 200.0f
#define CROUCH_HEIGHT 0.0f
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f
// Grounded drag
#define FRICTION .97f
// Increasing air drag, increases strafing speed
#define AIR_DRAG 0.95f
// Responsiveness for turning movement direction to looked direction
#define CONTROL 15.0f

#define NORMALIZE_INPUT 0

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Body structure
typedef struct {
  bool isGrounded;
  float headTimer;
  float walkLerp;
  float headLerp;
  Vector2 lean;
  Vector2 sensitivity;
  Vector2 lookRotation;
  Vector3 position;
  Vector3 velocity;
  Vector3 dir;
} Body;

typedef struct Player {
  Camera camera;
  Body body;
} Player;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdateCameraAngle(Player *player);
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold, Chunk *chunk_data);

Player InitPlayer(void);
void UpdatePlayer(Player *player, Chunk *chunk_data);
int GerCurrentChunk(Player *player);

#endif // PLAYER_H
