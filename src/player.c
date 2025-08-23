#include "player.h"
#include "raymath.h"
#include "resource_tracker.h"
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
  player.body.position = (Vector3){-10, N_VOXEL_Y + 2, -10};
  // (Vector3){(float)N_VOXEL_X / 2, N_VOXEL_Y + 2, (float)N_VOXEL_Z / 2};
  player.body.sensitivity = (Vector2){0.001f, 0.001f};
  player.body.collision_shape = (BoundingBox){
      (Vector3){player.body.position.x - PLAYER_COLLISION_SHAPE_X / 2,
                player.body.position.y,
                player.body.position.z - PLAYER_COLLISION_SHAPE_Z / 2},
      (Vector3){player.body.position.x + PLAYER_COLLISION_SHAPE_X / 2,
                player.body.position.y + PLAYER_COLLISION_SHAPE_Y,
                player.body.position.z + PLAYER_COLLISION_SHAPE_Z / 2}};

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

bool IsColliding(Voxel *voxel_data, BoundingBox box, Chunk current_chunk) {
  Vector3 chunk_world_offset = {
      (float)current_chunk.position.x_offset * N_VOXEL_X * VOXEL_SIZE, 0.0f,
      (float)current_chunk.position.z_offset * N_VOXEL_Z * VOXEL_SIZE};

  BoundingBox player_box_local = {
      Vector3Subtract(box.min, chunk_world_offset),
      Vector3Subtract(box.max, chunk_world_offset)};

  int min_x = (int)floorf(player_box_local.min.x / VOXEL_SIZE);
  int max_x = (int)ceilf(player_box_local.max.x / VOXEL_SIZE);
  int min_y = (int)floorf(player_box_local.min.y / VOXEL_SIZE);
  int max_y = (int)ceilf(player_box_local.max.y / VOXEL_SIZE);
  int min_z = (int)floorf(player_box_local.min.z / VOXEL_SIZE);
  int max_z = (int)ceilf(player_box_local.max.z / VOXEL_SIZE);

  for (int x = min_x; x < max_x; x++) {
    for (int y = min_y; y < max_y; y++) {
      for (int z = min_z; z < max_z; z++) {
        u64 index = GetVoxelIndex(x, y, z);
        if (index != -1 && Voxel_IsActive(voxel_data[index])) {
          BoundingBox voxel_box = GetVoxelBoundingBox(voxel_data[index]);
          if (CheckCollisionBoxes(player_box_local, voxel_box)) {
            return true;
          }
        }
      }
    }
  }
  return false;
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
             chunk_data[GetIndexCurrentChunk(player)]);

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
  return (BoundingBox){
      (Vector3){player_position.x - PLAYER_COLLISION_SHAPE_X / 2,
                player_position.y,
                player_position.z - PLAYER_COLLISION_SHAPE_Z / 2},
      (Vector3){player_position.x + PLAYER_COLLISION_SHAPE_X / 2,
                player_position.y + PLAYER_COLLISION_SHAPE_Y,
                player_position.z + PLAYER_COLLISION_SHAPE_Z / 2}};
}

void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold, Chunk current_chunk) {
  Vector2 input = (Vector2){(float)side, (float)-forward};

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

  float speed = Vector3DotProduct(hvel, body->dir);

  float maxSpeed = (crouchHold ? CROUCH_SPEED : MAX_SPEED);
  float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL * delta);
  hvel.x += body->dir.x * accel;
  hvel.z += body->dir.z * accel;

  body->velocity.x = hvel.x;
  body->velocity.z = hvel.z;

  Vector3 old_position = body->position;

  // Move on X axis
  body->position.x += body->velocity.x * delta;
  body->collision_shape = UpdateBodyCollisionShape(body->position);
  if (IsColliding(voxel_data, body->collision_shape, current_chunk)) {
    body->position.x = old_position.x;
    body->velocity.x = 0;
  }

  // Move on Z axis
  body->position.z += body->velocity.z * delta;
  body->collision_shape = UpdateBodyCollisionShape(body->position);
  if (IsColliding(voxel_data, body->collision_shape, current_chunk)) {
    body->position.z = old_position.z;
    body->velocity.z = 0;
  }

  // Move on Y axis
  body->position.y += body->velocity.y * delta;
  body->collision_shape = UpdateBodyCollisionShape(body->position);
  if (IsColliding(voxel_data, body->collision_shape, current_chunk)) {
    body->position.y = old_position.y;
    body->isGrounded = true;
    body->velocity.y = 0;
  } else {
    body->isGrounded = false;
  }

  // Update collision shape to final position
  body->collision_shape = UpdateBodyCollisionShape(body->position);
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

u32 GetIndexCurrentChunk(Player *player) {
  float chunk_size_x = N_VOXEL_X * VOXEL_SIZE;
  float chunk_size_z = N_VOXEL_Z * VOXEL_SIZE;

  int chunk_x_offset = floorf(player->body.position.x / chunk_size_x);
  int chunk_z_offset = floorf(player->body.position.z / chunk_size_z);

  int chunk_x_index = chunk_x_offset + (N_CHUNKS_X / 2);
  int chunk_z_index = chunk_z_offset + (N_CHUNKS_Z / 2);

  // Clamp chunk indices to be within the world boundaries
  if (chunk_x_index < 0)
    chunk_x_index = 0;
  if (chunk_x_index >= N_CHUNKS_X)
    chunk_x_index = N_CHUNKS_X - 1;
  if (chunk_z_index < 0)
    chunk_z_index = 0;
  if (chunk_z_index >= N_CHUNKS_Z)
    chunk_z_index = N_CHUNKS_Z - 1;

  return chunk_x_index * N_CHUNKS_Z + chunk_z_index;
}

s16Vector2D GetXZCurrentChunk(Player *player) {
  float chunk_size_x = N_VOXEL_X * VOXEL_SIZE;
  float chunk_size_z = N_VOXEL_Z * VOXEL_SIZE;

  return (s16Vector2D){floorf(player->body.position.x / chunk_size_x),
                       floorf(player->body.position.z / chunk_size_z)};
}
