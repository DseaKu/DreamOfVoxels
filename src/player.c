#include "player.h"
#include "debug_logger.h"
#include "raymath.h"
#include "resource_tracker.h"
#include "scene_voxel_game.h"
#include "std_includes.h"
#include "voxel.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

Player InitPlayer(void) {
  Player player = {0};
  player.camera.target = (Vector3){0.0f, 1.8f, 0.0f};
  player.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  player.camera.fovy = 60.0f;
  player.camera.projection = CAMERA_PERSPECTIVE;
  player.body.headLerp = STAND_HEIGHT;
  // N_VOXEL_Y to spawn above chunks
  player.body.position =
      (Vector3){(CHUNK_SIZE + 1) * VOXEL_SIZE * N_CHUNKS_X / 2, N_VOXEL_Y + 2,
                (CHUNK_SIZE + 1) * VOXEL_SIZE};
  // (Vector3){(float)N_VOXEL_X / 2, N_VOXEL_Y + 2, (float)N_VOXEL_Z / 2};
  player.body.sensitivity = (Vector2){0.001f, 0.001f};
  player.body.collision_shape = UpdateBodyCollisionShape(player.body.position);

  return player;
}

BoundingBox GetVoxelBoundingBox(Voxel v) {
  float x = (float)Voxel_GetPosX(v) * VOXEL_SIZE;
  float y = (float)Voxel_GetPosY(v) * VOXEL_SIZE;
  float z = (float)Voxel_GetPosZ(v) * VOXEL_SIZE;
  return (BoundingBox){
      (Vector3){x - HALF_VOXEL_SIZE, y - HALF_VOXEL_SIZE, z - HALF_VOXEL_SIZE},
      (Vector3){x + HALF_VOXEL_SIZE, y + HALF_VOXEL_SIZE, z + HALF_VOXEL_SIZE}};
}

bool AABB_Collision(Chunk *chunk_data, const Body body,
                    s64Vector3D target_voxel_offset) {
  StartPerformanceTracker("  └> Check Collision");
  // Get voxel from global map
  // Determine dir
  //
  //
  // I just want to know, if target v is active. if so just craete the boundbox
  // from tar voxel
  s64Vector3D target_voxel =
      (s64Vector3D){body.voxel_position.x + target_voxel_offset.x,
                    body.voxel_position.y + target_voxel_offset.y,
                    body.voxel_position.z + target_voxel_offset.z};
  if (IsVoxelActive_Global(chunk_data, target_voxel)) {

    u8 a=1;
    // create BoundingBox from target voxel
  }

  u64 tar_voxel;
  Voxel target_v; // = chunk_data etc..
  bool is_colliding = false;

  // if (Voxel_IsActive(v)) {
  //   // Transform player's world AABB to chunk's local AABB
  //   Vector3 chunk_world_offset = {
  //       (float)current_chunk.position.x * N_VOXEL_X * VOXEL_SIZE, 0.0f,
  //       (float)current_chunk.position.z * N_VOXEL_Z * VOXEL_SIZE};
  //
  //   BoundingBox player_box_local = {
  //       Vector3Subtract(body.collision_shape.min, chunk_world_offset),
  //       Vector3Subtract(body.collision_shape.max, chunk_world_offset)};
  //
  //   BoundingBox v_box_local = GetVoxelBoundingBox(v);
  //   BoundingBox a = body.collision_shape;
  //   if (CheckCollisionBoxes(v_box_local, body.collision_shape)) {
  //     is_colliding = true;
  //     SetDebugMessage("Collision at x:%d z:%d y:%d", x, z, y);
  //   }
  // }
  EndPerformanceTracker("  └> Check Collision");
  return is_colliding;
}

void UpdatePlayer(Player *player, Chunk *chunk_data) {
  StartPerformanceTracker("Update Player");

  Vector2 mouse_delta = GetMouseDelta();
  player->body.lookRotation.x -= mouse_delta.x * player->body.sensitivity.x;
  player->body.lookRotation.y += mouse_delta.y * player->body.sensitivity.y;

  char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
  char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
  bool crouching = IsKeyDown(KEY_LEFT_CONTROL);

  UpdateBody(&player->body, player->body.lookRotation.x, sideway, forward,
             IsKeyPressed(KEY_SPACE), crouching,
             chunk_data[GetIndexCurrentChunk(player)], chunk_data);

  float delta = GetFrameTime();
  player->body.headLerp =
      Lerp(player->body.headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT),
           20.0f * delta);
  player->camera.position = (Vector3){
      player->body.position.x,
      player->body.position.y + (BOTTOM_HEIGHT + player->body.headLerp),
      player->body.position.z,
  };

  if (player->body.isGrounded && ((forward != 0) || (sideway != 0))) {
    player->body.headTimer += delta * 3.0f;
    player->body.walkLerp = Lerp(player->body.walkLerp, 1.0f, 10.0f * delta);
    player->camera.fovy = Lerp(player->camera.fovy, 55.0f, 5.0f * delta);
  } else {
    player->body.walkLerp = Lerp(player->body.walkLerp, 0.0f, 10.0f * delta);
    player->camera.fovy = Lerp(player->camera.fovy, 60.0f, 5.0f * delta);
  }

  player->body.lean.x =
      Lerp(player->body.lean.x, sideway * 0.02f, 10.0f * delta);
  player->body.lean.y =
      Lerp(player->body.lean.y, forward * 0.015f, 10.0f * delta);
  EndPerformanceTracker("Update Player");
}
BoundingBox UpdateBodyCollisionShape(Vector3 player_position) {

  float x = player_position.x;
  float z = player_position.z;
  float y = player_position.y;

  return (BoundingBox){(Vector3){x - HALF_PLAYER_COLLISION_SHAPE_X,
                                 y - HALF_PLAYER_COLLISION_SHAPE_Y,
                                 -HALF_PLAYER_COLLISION_SHAPE_X},
                       (Vector3){x + HALF_PLAYER_COLLISION_SHAPE_X,
                                 y + HALF_PLAYER_COLLISION_SHAPE_Y,
                                 +HALF_PLAYER_COLLISION_SHAPE_X}};
}

void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold, Chunk current_chunk,
                Chunk *chunk_data) {
  Vector2 input = (Vector2){(float)side, (float)-forward};

  StartPerformanceTracker("UpdateBody");

  // Update player's voxel position
  body->voxel_position.x = (int)floorf(body->position.x / VOXEL_SIZE);
  body->voxel_position.y = (int)floorf(body->position.y / VOXEL_SIZE);
  body->voxel_position.z = (int)floorf(body->position.z / VOXEL_SIZE);

  Voxel *voxel_data = current_chunk.p_voxel_data;
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
  // acceleration constant, a Player can make the speed faster by bringing
  // the direction closer to horizontal velocity angle More info here:
  // https://youtu.be/v3zT3Z5apaM?t=165
  float maxSpeed = (crouchHold ? CROUCH_SPEED : MAX_SPEED);
  float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL * delta);
  hvel.x += body->dir.x * accel;
  hvel.z += body->dir.z * accel;

  body->velocity.x = hvel.x;
  body->velocity.z = hvel.z;

  body->collision_shape = UpdateBodyCollisionShape(body->position);
  // If player moves xz direction, check for collision
  if (desiredDir.x != 0) {
    if (desiredDir.x < 0) {
      if (AABB_Collision(chunk_data, *body, (s64Vector3D){-1, 0, 0})) {
        body->velocity.x = 0.0f;
      }
    } else {
      if (AABB_Collision(chunk_data, *body, (s64Vector3D){1, 0, 0})) {
        body->velocity.x = 0.0f;
      }
    }
  }

  // }
  Vector3 new_position =
      Vector3Add(body->position, Vector3Scale(body->velocity, delta));
  // Update body position + collision shape
  body->position = new_position;
  body->collision_shape = UpdateBodyCollisionShape(body->position);

  // Fancy collision system against the floor
  if (body->position.y <= 0.0f) {
    body->position.y = 0.0f;
    body->velocity.y = 0.0f;
    body->isGrounded = true; // Enable jumping
  }
  EndPerformanceTracker("UpdateBody");
}

// Update camera
void UpdateCameraAngle(Player *player) {
  StartPerformanceTracker("Update Camera Angele");
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
  EndPerformanceTracker("Update Camera Angele");
}
