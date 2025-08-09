#include <float.h>
#include "scenes/scene_3d_game.h"
#include "sprites/block.h"
#include "sprites/player.h"

int Scene3DGame(void) {

  const int screenWidth = MAX_SCREEN_WIDTH;
  const int screenHeight = MAX_SCREEN_HEIGHT;

  InitWindow(screenWidth, screenHeight, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);

  Player player = InitPlayer();
  Block blocks[MAX_BLOCKS] = {0};
  int blockCount = 0;
  CreatePlaneGround(blocks, &blockCount);

  while (!WindowShouldClose()) {
    // Update
    UpdatePlayer(&player);

    // Block placement and removal
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      Ray ray = {player.camera.position,
                 Vector3Subtract(player.camera.target, player.camera.position)};

      RayCollision closestCollision = {0};
      closestCollision.distance = FLT_MAX;
      closestCollision.hit = false;

      int closestBlock = -1;

      for (int i = 0; i < blockCount; i++) {
        if (blocks[i].active) {
          BoundingBox bb = {Vector3SubtractValue(blocks[i].position, HALF_BLOCK_SIZE),
                            Vector3AddValue(blocks[i].position, HALF_BLOCK_SIZE)};
          RayCollision collision = GetRayCollisionBox(ray, bb);

          if (collision.hit && collision.distance < closestCollision.distance) {
            closestCollision = collision;
            closestBlock = i;
          }
        }
      }

      if (closestBlock != -1) {
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
          if (blockCount < MAX_BLOCKS) {
            blocks[blockCount].position = Vector3Add(
                blocks[closestBlock].position, closestCollision.normal);
            blocks[blockCount].active = true;
            blockCount++;
          }
        } else {
          blocks[closestBlock].active = false;
        }
      }
    }

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);

    // Draw blocks
    for (int i = 0; i < blockCount; i++) {
      if (blocks[i].active) {
        DrawCube(blocks[i].position, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, GRAY);
        DrawCubeWires(blocks[i].position, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, MAROON);
      }
    }

    EndMode3D();

    DrawText("Welcome to Raylib Minecraft!", 10, 10, 20, DARKGRAY);
    DrawText("Right-click to place a block", 10, 40, 10, DARKGRAY);
    DrawText("Left-click to remove a block", 10, 60, 10, DARKGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}