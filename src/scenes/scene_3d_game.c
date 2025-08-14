#include "scenes/scene_3d_game.h"
#include "sprites/block.h"
#include "sprites/player.h"
#include "utils/performance.h"

int Scene3DGame(void) {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);

  Player player = InitPlayer();

  Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX] = {0};
  InitBlocks(blocks);
  SearchScope scope = {0, PLAYGROUND_X_MAX, 0, PLAYGROUND_Y_MAX,
                       0, PLAYGROUND_Z_MAX};
  UpdateAllBlockFaces(blocks, scope);

  // Use a standard cube mesh
  Texture2D texture = LoadTexture("assets/log.png");
  Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  Model model = LoadModelFromMesh(mesh);
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdateAllBlockFaces(blocks, scope);
    UpdatePlayer(&player);
    EndPerformanceTracker("UpdateLoop");

    // Block placement
    StartPerformanceTracker("BlockPlacment");
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      RemoveBlock(blocks);
    }
    EndPerformanceTracker("BlockPlacment");

    // Rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawFPS(screen_width - 100, 10);
    BeginMode3D(player.camera);

    StartPerformanceTracker("DrawingBlocks");
    DrawCubeFace(blocks, model);
    EndPerformanceTracker("DrawingBlocks");
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);

    EndMode3D();
    EndDrawing();
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}
