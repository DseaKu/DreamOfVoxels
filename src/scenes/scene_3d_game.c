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

  Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX] = {0};

  // Set all visible faces to true
  UpdateAllBlockFaces(blocks);

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Player movement
    UpdatePlayer(&player);

    // Block placement
    StartPerformanceTracker("BlockPlacment");
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      RemoveBlock(blocks);
      UpdateAllBlockFaces(blocks);
    }
    EndPerformanceTracker("BlockPlacment");

    // Begin draw
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    DrawFPS(screen_width - 100, 10);
    BeginMode3D(player.camera);

    // Draw blocks
    StartPerformanceTracker("DrawingBlocks");
    DrawCubeFace(blocks);
    EndPerformanceTracker("DrawingBlocks");

    // End draw
    EndMode3D();
    EndDrawing();
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}