#include "scenes/scene_3d_game.h"
#include "sprites/block.h"
#include "sprites/player.h"
#include "utils/performance.h"

int Scene3DGame(void) {

  const int screen_width = MAX_SCREEN_WIDTH;
  const int screen_height = MAX_SCREEN_HEIGHT;

  InitWindow(screen_width, screen_height, "Raylib ");
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

    DrawCircle(screen_width / 2, screen_height / 2, 2, BLACK);

    DrawFPS(screen_width - 100, 10);

    EndDrawing();

    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}
