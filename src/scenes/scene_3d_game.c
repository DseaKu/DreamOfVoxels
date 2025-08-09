#include "../../includes/scenes/scene_3d_game.h"

int scene_3d_game(void) {

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Raylib ");

  Player player = {0};
  player.camera.position = (Vector3){0.0f, 2.0f, 4.0f};
  player.camera.target = (Vector3){0.0f, 1.8f, 0.0f};
  player.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  player.camera.fovy = 60.0f;
  player.camera.projection = CAMERA_PERSPECTIVE;
  player.speed = 0.1f;

  Block blocks[MAX_BLOCKS] = {0};
  int blockCount = 0;

  // Create a ground plane
  for (int x = -8; x < 8; x++) {
    for (int z = -8; z < 8; z++) {
      blocks[blockCount].position = (Vector3){(float)x, 0.0f, (float)z};
      blocks[blockCount].active = true;
      blockCount++;
    }
  }

  DisableCursor();
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // Update
    UpdateCamera(&player.camera, CAMERA_FIRST_PERSON);

    // Block placement and removal
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      Ray ray = {player.camera.position,
                 Vector3Subtract(player.camera.target, player.camera.position)};

      RayCollision closestCollision = {0};
      closestCollision.distance = 3.402823466e+38F;
      closestCollision.hit = false;

      int closestBlock = -1;

      for (int i = 0; i < blockCount; i++) {
        if (blocks[i].active) {
          BoundingBox bb = {Vector3SubtractValue(blocks[i].position, 0.5f),
                            Vector3AddValue(blocks[i].position, 0.5f)};
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
        DrawCube(blocks[i].position, 1.0f, 1.0f, 1.0f, GRAY);
        DrawCubeWires(blocks[i].position, 1.0f, 1.0f, 1.0f, MAROON);
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
