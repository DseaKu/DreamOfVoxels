#include "scenes/scene_3d_game.h"
#include "core/block.h"
#include "sprites/player.h"
#include "utils/rescource_tracker.h"
#include <raylib.h>

int Scene3DGame(void) {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);

  Player player = InitPlayer();

  Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX] = {0};
  InitBlocks(blocks);

  // Use a standard cube mesh
  // Texture2D texture = LoadTexture("assets/log.png");
  // Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  // Model model = LoadModelFromMesh(mesh);
  // model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdatePlayer(&player);
    EndPerformanceTracker("UpdateLoop");

    // Block placement
    // StartPerformanceTracker("BlockPlacment");
    // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    // RemoveBlock(blocks);
    // }
    // EndPerformanceTracker("BlockPlacment");

    // Rendering 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    DrawGrid(100, 1.0f);
    // StartPerformanceTracker("DrawingBlocks");
    // EndPerformanceTracker("DrawingBlocks");
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
