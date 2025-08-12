#include "scenes/scene_3d_game.h"
#include "sprites/block.h"
#include "sprites/player.h"
#include "utils/performance.h"

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
    StartPerformanceTracker("CompleteLoop");

    // Player movement
    UpdatePlayer(&player);

    StartPerformanceTracker("BlockPlacment");
    // Block placement
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      BlockPlacment(MAX_PLACMENT_DISTANCE, &player, blocks, &blockCount,
                    BLOCK_SIZE);
    }
    EndPerformanceTracker("BlockPlacment");

    // Draw
    StartPerformanceTracker("DrawingBlocks");
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);

    // Draw blocks
    DrawBlocks(blocks, blockCount, BLOCK_SIZE);

    EndMode3D();

    DrawFPS(screenWidth - 100, 10);

    EndDrawing();

    EndPerformanceTracker("DrawingBlocks");
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}
