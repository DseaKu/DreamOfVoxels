#include "player.h"
#include "raymath.h"
#include <raylib.h>

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
// Vector2 sensitivity = {0.001f, 0.001f};
// Vector2 lookRotation = {0};
// float headTimer = 0.0f;
// float walkLerp = 0.0f;
// float headLerp = STAND_HEIGHT;
// Vector2 lean = {0};

Player InitPlayer(void) {
  Player player = {0};
  player.camera.target = (Vector3){0.0f, 1.8f, 0.0f};
  player.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  player.camera.fovy = 60.0f;
  player.camera.projection = CAMERA_PERSPECTIVE;
  player.body.headLerp = STAND_HEIGHT;
  player.body.sensitivity.x = 0.001f;
  player.body.sensitivity.y = 0.001f;
  return player;
}

void UpdatePlayer(Player *player) {
  UpdateCamera(&player->camera, CAMERA_FIRST_PERSON);
}
//----------------------------------------------------------------------------------
// Module functions definition
//----------------------------------------------------------------------------------
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold) {
  Vector2 input = (Vector2){(float)side, (float)-forward};

  // Slow down diagonal movement
  if ((side != 0) && (forward != 0))
    input = Vector2Normalize(input);

  float delta = GetFrameTime();

  if (!body->isGrounded)
    body->velocity.y -= GRAVITY * delta;

  if (body->isGrounded && jumpPressed) {
    body->velocity.y = JUMP_FORCE;
    body->isGrounded = false;

    // Sound can be played at this moment
    // SetSoundPitch(fxJump, 1.0f + (GetRandomValue(-100, 100)*0.001));
    // PlaySound(fxJump);
  }

  Vector3 front = (Vector3){sin(rot), 0.f, cos(rot)};
  Vector3 right = (Vector3){cos(-rot), 0.f, sin(-rot)};

  Vector3 desiredDir = (Vector3){
      input.x * right.x + input.y * front.x,
      0.0f,
      input.x * right.z + input.y * front.z,
  };
  body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL * delta);

  float decel = (body->isGrounded ? FRICTION : AIR_DRAG);
  Vector3 hvel =
      (Vector3){body->velocity.x * decel, 0.0f, body->velocity.z * decel};

  float hvelLength = Vector3Length(hvel); // Magnitude
  if (hvelLength < (MAX_SPEED * 0.01f))
    hvel = (Vector3){0};

  // This is what creates strafing
  float speed = Vector3DotProduct(hvel, body->dir);

  // Whenever the amount of acceleration to add is clamped by the maximum
  // acceleration constant, a Player can make the speed faster by bringing the
  // direction closer to horizontal velocity angle More info here:
  // https://youtu.be/v3zT3Z5apaM?t=165
  float maxSpeed = (crouchHold ? CROUCH_SPEED : MAX_SPEED);
  float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL * delta);
  hvel.x += body->dir.x * accel;
  hvel.z += body->dir.z * accel;

  body->velocity.x = hvel.x;
  body->velocity.z = hvel.z;

  body->position.x += body->velocity.x * delta;
  body->position.y += body->velocity.y * delta;
  body->position.z += body->velocity.z * delta;

  // Fancy collision system against the floor
  if (body->position.y <= 0.0f) {
    body->position.y = 0.0f;
    body->velocity.y = 0.0f;
    body->isGrounded = true; // Enable jumping
  }
}

// Update camera
void UpdateCameraAngle(Player *player) {
  const Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};
  const Vector3 targetOffset = (Vector3){0.0f, 0.0f, -1.0f};

  // Left and right
  Vector3 yaw =
      Vector3RotateByAxisAngle(targetOffset, up, player->body.lookRotation.x);

  // Clamp view up
  float maxAngleUp = Vector3Angle(up, yaw);
  maxAngleUp -= 0.001f; // Avoid numerical errors
  if (-(player->body.lookRotation.y) > maxAngleUp) {
    player->body.lookRotation.y = -maxAngleUp;
  }

  // Clamp view down
  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f;  // Downwards angle is negative
  maxAngleDown += 0.001f; // Avoid numerical errors
  if (-(player->body.lookRotation.y) < maxAngleDown) {
    player->body.lookRotation.y = -maxAngleDown;
  }

  // Up and down
  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  // Rotate view vector around right axis
  Vector3 pitch = Vector3RotateByAxisAngle(
      yaw, right, -player->body.lookRotation.y - player->body.lean.y);

  // Head animation
  // Rotate up direction around forward axis
  float headSin = sin(player->body.headTimer * PI);
  float headCos = cos(player->body.headTimer * PI);
  const float stepRotation = 0.01f;
  player->camera.up = Vector3RotateByAxisAngle(
      up, pitch, headSin * stepRotation + player->body.lean.x);

  // Camera BOB
  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
  bobbing.y = fabsf(headCos * bobUp);

  player->camera.position = Vector3Add(
      player->camera.position, Vector3Scale(bobbing, player->body.walkLerp));
  player->camera.target = Vector3Add(player->camera.position, pitch);
}
