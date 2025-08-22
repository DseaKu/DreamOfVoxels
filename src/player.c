#include "player.h"
#include "raymath.h"
#include "resource_tracker.h"
#include "voxel.h"
#include <math.h>
#include <raylib.h>

Player InitPlayer(void) {
  Player player = {0};
  player.camera.target = (Vector3){0.0f, 1.8f, 0.0f};
  player.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  player.camera.fovy = 60.0f;
  player.camera.projection = CAMERA_PERSPECTIVE;
  player.body.headLerp = STAND_HEIGHT;
  // N_VOXEL_Y to spawn above chunks
  player.body.position = (Vector3){2, N_VOXEL_Y + 2, 2};
  // (Vector3){(float)N_VOXEL_X / 2, N_VOXEL_Y + 2, (float)N_VOXEL_Z / 2};
  player.body.sensitivity = (Vector2){0.001f, 0.001f};
  return player;
}

static bool IsColliding(Voxel *voxel_data, Vector3 position,
                         Chunk current_chunk) {
  StartPerformanceTracker("  └-> Check Collision");
  for (int i = 0; i < VOXELS_IN_TOTAL; i++) {
    Voxel v = voxel_data[i];

    if (Voxel_IsActive(v)) {
      // Calculate the world position of the voxel
      float voxel_world_x =
          (current_chunk.position.x_offset * N_VOXEL_X) + Voxel_GetPosX(v);
      float voxel_world_z =
          (current_chunk.position.z_offset * N_VOXEL_Z) + Voxel_GetPosZ(v);
      BoundingBox voxel_box = {
          (Vector3){voxel_world_x - 0.5f, Voxel_GetPosY(v) - 0.5f,
                    voxel_world_z - 0.5f},
          (Vector3){voxel_world_x + 0.5f, Voxel_GetPosY(v) + 0.5f,
                    voxel_world_z + 0.5f}};
      BoundingBox player_box = {
          (Vector3){position.x - 0.25f, position.y, position.z - 0.25f},
          (Vector3){position.x + 0.25f, position.y + 1.8f, position.z + 0.25f}};
      if (CheckCollisionBoxes(player_box, voxel_box)) {
        return true;
      }
    }
  }
  EndPerformanceTracker("  └-> Check Collision");
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

  Vector3 new_position = body->position;
  new_position.x += body->velocity.x * delta;
  if (IsColliding(voxel_data, new_position, current_chunk)) {
    new_position.x = body->position.x;
  }

  new_position.y += body->velocity.y * delta;
  if (IsColliding(voxel_data, new_position, current_chunk)) {
    new_position.y = body->position.y;
    body->velocity.y = 0;
    body->isGrounded = true;
  }

  new_position.z += body->velocity.z * delta;
  if (IsColliding(voxel_data, new_position, current_chunk)) {
    new_position.z = body->position.z;
  }

  body->position = new_position;

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

SnVector2D GetXZCurrentChunk(Player *player) {
  float chunk_size_x = N_VOXEL_X * VOXEL_SIZE;
  float chunk_size_z = N_VOXEL_Z * VOXEL_SIZE;

  return (SnVector2D){floorf(player->body.position.x / chunk_size_x),
                      floorf(player->body.position.z / chunk_size_z)};
}
