#include "scenes/scene_3d_game.h"
#include "core/voxel.h"
#include "rlgl.h"
#include "sprites/player.h"
#include "utils/resource_tracker.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

void RenderVoxelFaces(Voxel *voxel_data);

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

  bool is_visibility_updatable = false;

  do {
    StartPerformanceTracker("CompleteLoop");

    // Update objects
    UpdatePlayer(&player);
    if (is_visibility_updatable) {
      UpdateVisibility(voxel_data);
    }

    // Draw 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    RenderVoxelFaces(voxel_data);
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

  free(voxel_data);
  CloseWindow();
  PrintPerformanceTrackers();
  return 0;
}

void RenderVoxelFaces(Voxel *voxel_data) {
  StartPerformanceTracker("RenderVoxelFaces");
  rlBegin(RL_QUADS);

  for (u64 index = 0; index < NUMBER_OF_VOXELS; index++) {
    Voxel v = voxel_data[index];

    // Skip EMPTY voxels
    if (((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID) == EMPTY) {
      continue;
    }

    u8 visible_faces = (v >> VOXEL_SHIFT_FACE) & VOXEL_MASK_FACE;

    // If no faces are visible, skip.
    if (visible_faces == 0) {
      continue;
    }

    // Get voxel position
    float x = (float)Voxel_GetPosX(v);
    float y = (float)Voxel_GetPosY(v);
    float z = (float)Voxel_GetPosZ(v);

    // Determine color based on VoxelID
    Color color = WHITE;
    VoxelID id = (VoxelID)((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID);
    if (id == DIRT) {
      color = (Color){139, 69, 19, 255}; // Brown
    } else if (id == WATER) {
      color = (Color){0, 0, 255, 128}; // Semi-transparent blue
    }

    rlColor4ub(color.r, color.g, color.b, color.a);

    // Check and draw each visible face (counter-clockwise order)
    if (visible_faces & FACE_DIR_POS_X) { // Right face
      rlNormal3f(1.0f, 0.0f, 0.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
      rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
      rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
      rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_NEG_X) { // Left face
      rlNormal3f(-1.0f, 0.0f, 0.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
      rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
      rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
      rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
    }
    if (visible_faces & FACE_DIR_POS_Y) { // Top face
      rlNormal3f(0.0f, 1.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
      rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
      rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
      rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_NEG_Y) { // Bottom face
      rlNormal3f(0.0f, -1.0f, 0.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
      rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
      rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
      rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_POS_Z) { // Front face
      rlNormal3f(0.0f, 0.0f, 1.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
      rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
      rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
      rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_NEG_Z) { // Back face
      rlNormal3f(0.0f, 0.0f, -1.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
      rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
      rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
      rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
    }
  }
  rlEnd();
  EndPerformanceTracker("RenderVoxelFaces");
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
  DrawText("X-Axis", 30, 10, 10, RED);
  DrawText("Y-Axis", 30, 20, 10, GREEN);
  DrawText("Z-Axis", 30, 30, 10, BLUE);
  DrawFPS(screen_width - 100, 10);
  EndPerformanceTracker("Draw 2D debug information");
}
