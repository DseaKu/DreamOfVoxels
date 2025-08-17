#include "scenes/scene_3d_game.h"
#include "core/voxel.h"
#include "sprites/player.h"
#include "utils/resource_tracker.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

void RenderVoxelFaces(Voxel *voxel_data, Texture2D texture);

int Scene3DGame() {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  // INIT
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Voxel *voxel_data = (Voxel *)calloc(NUMBER_OF_VOXELS, sizeof(Voxel));
  InitVoxel(voxel_data);

  UpdateVisibility(voxel_data);

  Texture2D dirt_texture = LoadTexture("assets/log.png");

  bool is_visibility_updatable = false;

  do {
    StartPerformanceTracker("CompleteLoop");
    // Process events
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

      RemoveVoxel(voxel_data, &player, screen_width, screen_height,
                  MAX_PLAYER_RANGE);
      is_visibility_updatable = true;
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      TryPlaceVoxel(voxel_data, &player, screen_width, screen_height,
                    MAX_PLAYER_RANGE);
      is_visibility_updatable = true;
    }

    // Update objects
    UpdatePlayer(&player);
    if (is_visibility_updatable) {
      UpdateVisibility(voxel_data);
      is_visibility_updatable = false;
    }

    // Draw 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    RenderVoxelFaces(voxel_data, dirt_texture);
    Draw3DDebugInformation(screen_width, screen_height);
    EndMode3D();

    // Draw 2D
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    Draw2DDebugInformation(screen_width, screen_height);

    // End drawing
    EndDrawing();

    // voxels[1437].id = EMPTY;
    EndPerformanceTracker("CompleteLoop");
  } while (!WindowShouldClose());
  // } while (WindowShouldClose());

  UnloadTexture(dirt_texture);
  free(voxel_data);
  CloseWindow();
  PrintPerformanceTrackers();
  return 0;
}

void Draw3DDebugInformation(int screen_width, int screen_height) {

  StartPerformanceTracker("Draw 3D debug information");
  DrawGrid(100, 1.0f);
  // Draw coordinate system
  DrawCylinderEx((Vector3){-1.0f, 0.0f, -1.0f}, (Vector3){50.0f, 0.0f, 0.0f},
                 0.01f, 0.01f, 24, RED);
  DrawCylinderEx((Vector3){-1.0f, 0.0f, -1.0f}, (Vector3){0.0f, 50.0f, 0.0f},
                 0.01f, 0.01f, 24, GREEN);
  DrawCylinderEx((Vector3){-1.0f, 0.0f, -1.0f}, (Vector3){0.0f, 0.0f, 50.0f},
                 0.01f, 0.01f, 24, BLUE);
  EndPerformanceTracker("Draw 3D debug information");
}
void Draw2DDebugInformation(int screen_width, int screen_height) {
  StartPerformanceTracker("Draw 2D debug information");
  DrawText("X-Axis", 30, 10, 10, RED);
  DrawText("Y-Axis", 30, 20, 10, GREEN);
  DrawText("Z-Axis", 30, 30, 10, BLUE);
  DrawFPS(screen_width - 100, 10);
  EndPerformanceTracker("Draw 2D debug information");
}
