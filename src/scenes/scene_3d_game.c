#include "scenes/scene_3d_game.h"
#include "core/voxel.h"
#include "sprites/player.h"
#include "utils/rescource_tracker.h"
#include <raylib.h>

int Scene3DGame() {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  // INIT
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Voxel voxels[VOXEL_XYZ] = {0};
  InitVoxel(voxels);

  // // Use a standard cube mesh
  // Texture2D texture = LoadTexture("assets/log.png");
  // Model cube_model = LoadModelFromMesh(BuildVoxelFaceMesh(voxels));
  // cube_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdateVoxel(voxels);
    UpdatePlayer(&player);

    EndPerformanceTracker("UpdateLoop");

    // Draw 3D
    StartPerformanceTracker("Draw 3D");
    BeginDrawing();
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);
    // DrawModel(cube_model, (Vector3){1.0f, 0.0f, 1.0f}, 1.0f, WHITE);
    DrawVoxelSimple(voxels);
    EndPerformanceTracker("Draw 3D");

    // Draw debug information
    StartPerformanceTracker("Draw debug information");
    DrawDebugInformation(screen_width, screen_height);
    EndPerformanceTracker("Draw debug information");

    // End drawing
    EndMode3D();
    EndDrawing();
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();

  PrintPerformanceTrackers();

  return 0;
}

void DrawDebugInformation(int screen_width, int screen_height) {
  DrawGrid(100, 1.0f);
  // Draw coordinate system
  DrawLine3D((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){50.0f, 0.0f, 0.0f}, RED);
  DrawLine3D((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 50.0f, 0.0f}, GREEN);
  DrawLine3D((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, 50.0f}, BLUE);

  DrawFPS(screen_width - 100, 10);
}
