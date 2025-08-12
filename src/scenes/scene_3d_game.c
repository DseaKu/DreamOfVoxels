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

    // Player movement
    UpdatePlayer(&player);

    // Block placement
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      BlockPlacment(MAX_PLACMENT_DISTANCE, &player, blocks, &blockCount,
                    BLOCK_SIZE);
    }

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);

    // Draw blocks
    for (int i = 0; i < blockCount; i++) {
      if (blocks[i].active) {
        DrawCube(blocks[i].position, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, GRAY);
        DrawCubeWires(blocks[i].position, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                      MAROON);
      }
    }

    EndMode3D();

    DrawText("Lol!", 10, 10, 20, DARKGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
