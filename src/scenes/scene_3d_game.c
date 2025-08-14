#include "scenes/scene_3d_game.h"
#include "core/voxel.h"
#include "sprites/player.h"
#include "utils/rescource_tracker.h"
#include <raylib.h>

int Scene3DGame(void) {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  // INIT
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Voxel voxels[VOXEL_XYZ] = {0};
  InitVoxels(voxels);

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdatePlayer(&player);
    EndPerformanceTracker("UpdateLoop");

    // Rendering 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    DrawVoxel(voxels);

    DrawGrid(100, 1.0f);
    EndMode3D();

    // Rendering 2D
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    DrawFPS(screen_width - 100, 10);
    EndDrawing();
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}
