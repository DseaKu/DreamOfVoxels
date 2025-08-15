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
  InitVoxel(voxels);

  voxels[47].id = SOLID;
  voxels[10].id = SOLID;

  // Use a standard cube mesh
  Texture2D texture = LoadTexture("assets/log.png");
  Model cube_model = LoadModelFromMesh(BuildVoxelFaceMesh(voxels));
  cube_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdatePlayer(&player);
    EndPerformanceTracker("UpdateLoop");

    // Draw 3D
    StartPerformanceTracker("Draw 3D");
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    // Draw mesh
    // DrawModel(cube_model, (Vector3){1.0f, 0.0f, 1.0f}, 1.0f, WHITE);
    DrawCubesNaive(voxels);

    DrawGrid(100, 1.0f);
    EndPerformanceTracker("Draw 3D");
    EndMode3D();

    // Rendering 2D
    StartPerformanceTracker("Draw 2D");
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    DrawFPS(screen_width - 100, 10);
    EndPerformanceTracker("Draw 2D");
    EndDrawing();
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}
