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

    // Block placement
    StartPerformanceTracker("BlockPlacment");
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      BlockPlacment(MAX_PLACMENT_DISTANCE, &player, blocks, &blockCount,
                    BLOCK_SIZE);
    }
    EndPerformanceTracker("BlockPlacment");

    // Begin draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Begin 3D drawing
    BeginMode3D(player.camera);

    // Draw blocks
    StartPerformanceTracker("DrawingBlocks");
    DrawBlocks(blocks, blockCount, BLOCK_SIZE);
    EndPerformanceTracker("DrawingBlocks");

    EndMode3D();

    DrawCircle(screenWidth / 2, screenHeight / 2, 2, BLACK);

    DrawFPS(screenWidth - 100, 10);

    EndDrawing();

    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}
