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

  Vector3 offset0 = {0.0f, 0.0f, 0.0f};
  Model model0 = LoadModelFromMesh(GenMeshCustom(offset0));

  Vector3 offset1 = {5.0f, 1.0f, 3.0f};
  Model model1 = LoadModelFromMesh(GenMeshCustom(offset1));

  // Use a standard cube mesh
  Texture2D texture = LoadTexture("assets/log.png");
  Model cube_model = LoadModelFromMesh(BuildCubesMeshSimple(voxels));
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

    // Draw cube
    DrawModel(cube_model, (Vector3){1.0f, 0.0f, 1.0f}, 1.0f, WHITE);
    // DrawCubesNaive(voxels);
    // DrawModel(model0, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, RED);
    // DrawModel(model1, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, BLUE);

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
