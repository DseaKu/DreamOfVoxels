#include "scenes/scene_3d_game.h"
#include "core/voxel.h"
#include "sprites/player.h"
<<<<<<< HEAD
#include "utils/mesher.h"
#include "utils/performance.h"
=======
#include "utils/rescource_tracker.h"
#include <raylib.h>
>>>>>>> main

int Scene3DGame(void) {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  // INIT
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
<<<<<<< HEAD

  Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX] = {0};
  InitBlocks(blocks);

  Texture2D texture = LoadTexture("assets/log.png");
  Model model = {0};
  bool mesh_is_dirty = true;
=======
  Voxel voxels[VOXEL_XYZ] = {0};
  InitVoxel(voxels);
>>>>>>> main

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    if (mesh_is_dirty) {
      UnloadModel(model);
      Mesh mesh = GenerateGreedyMesh(blocks);
      model = LoadModelFromMesh(mesh);
      model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
      mesh_is_dirty = false;
    }

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdatePlayer(&player);
    EndPerformanceTracker("UpdateLoop");

<<<<<<< HEAD
    // Block placement
    StartPerformanceTracker("BlockPlacment");
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      RemoveBlock(blocks, &player);
      mesh_is_dirty = true;
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      AddBlock(blocks, &player);
      mesh_is_dirty = true;
    }
    EndPerformanceTracker("BlockPlacment");

=======
>>>>>>> main
    // Rendering 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

<<<<<<< HEAD
    StartPerformanceTracker("DrawingBlocks");
    DrawModel(model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
    EndPerformanceTracker("DrawingBlocks");
=======
    DrawVoxel(voxels);

    DrawGrid(100, 1.0f);
>>>>>>> main
    EndMode3D();

    // Rendering 2D
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    DrawFPS(screen_width - 100, 10);
    EndDrawing();
    EndPerformanceTracker("CompleteLoop");
  }

  UnloadModel(model);
  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}