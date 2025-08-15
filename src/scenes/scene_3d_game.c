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

  // Use a standard cube mesh
  Vector3 model_location = {-0.5f, -0.5f, -0.5f};
  Texture2D texture = LoadTexture("assets/log.png");
  Model voxel_model = LoadModelFromMesh(BuildVoxelFaceMesh(voxels));
  voxel_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  while (!WindowShouldClose()) {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    StartPerformanceTracker("UpdateLoop");
    UpdateVoxel(voxels);
    UpdatePlayer(&player);
    EndPerformanceTracker("UpdateLoop");

    // Draw 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);
    StartPerformanceTracker("Draw 3D");
    // DrawModel(voxel_model, model_location, VOXEL_SIZE, WHITE);
    DrawModel(voxel_model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
    EndPerformanceTracker("Draw 3D");
    Draw3DDebugInformation(screen_width, screen_height);
    EndMode3D();

    // Draw 2D
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    Draw2DDebugInformation(screen_width, screen_height);

    // End drawing
    EndDrawing();

    // voxels[1437].id = EMPTY;
    EndPerformanceTracker("CompleteLoop");
  }

  CloseWindow();
  PrintPerformanceTrackers();
  return 0;
}

void Draw3DDebugInformation(int screen_width, int screen_height) {

  StartPerformanceTracker("Draw 3D debug information");
  DrawGrid(100, 1.0f);
  // Draw coordinate system
  DrawCylinderEx((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){50.0f, 0.0f, 0.0f},
                 0.01f, 0.01f, 24, RED);
  DrawCylinderEx((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 50.0f, 0.0f},
                 0.01f, 0.01f, 24, GREEN);
  DrawCylinderEx((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, 50.0f},
                 0.01f, 0.01f, 24, BLUE);
  EndPerformanceTracker("Draw 3D debug information");
}
void Draw2DDebugInformation(int screen_width, int screen_height) {
  StartPerformanceTracker("Draw 2D debug information");
  DrawText("X-Axsis", 30, 10, 10, RED);
  DrawText("Y-Axsis", 30, 20, 10, GREEN);
  DrawText("Z-Axsis", 30, 30, 10, BLUE);
  DrawFPS(screen_width - 100, 10);
  EndPerformanceTracker("Draw 2D debug information");
}
