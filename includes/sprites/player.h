#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Movement constants
#define GRAVITY 32.0f
#define MAX_SPEED 20.0f
#define CROUCH_SPEED 5.0f
#define JUMP_FORCE 12.0f
#define MAX_ACCEL 150.0f
// Grounded drag
#define FRICTION 0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG 0.98f
// Responsiveness for turning movement direction to looked direction
#define CONTROL 15.0f
#define CROUCH_HEIGHT 0.0f
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f

#define NORMALIZE_INPUT 0

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Body structure
typedef struct {
  Vector3 position;
  Vector3 velocity;
  Vector3 dir;
  bool isGrounded;
} Body;

typedef struct Player {
  Camera camera;
  Body body;
} Player;
//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern Vector2 sensitivity;
extern Vector2 lookRotation;
extern float headTimer;
extern float walkLerp;
extern float headLerp;
extern Vector2 lean;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdateCameraAngle(Player *player);
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold);

Player InitPlayer(void);
void UpdatePlayer(Player *player);

#endif // PLAYER_H
