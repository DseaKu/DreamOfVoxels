#include "scene_3d_game.h"
#include "player.h"
#include "resource_tracker.h"
#include "voxel.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

int Scene3DGame() {

  //----------------------------------------------------------------------------------
  // Init
  //----------------------------------------------------------------------------------
  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();

  // SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Chunk *p_chunk_data = InitChunks();

  Material material = LoadMaterialDefault();
  Image texture_atlas_img = LoadImage("assets/texture_atlas.png");
  material.maps[MATERIAL_MAP_DIFFUSE].texture =
      LoadTextureFromImage(texture_atlas_img);
  UnloadImage(texture_atlas_img);

  bool is_visibility_updatable = true;

  //----------------------------------------------------------------------------------
  // Game loop
  //----------------------------------------------------------------------------------
  do {
    StartPerformanceTracker("CompleteLoop");

    //----------------------------------------------------------------------------------
    // Get and process inputs
    //----------------------------------------------------------------------------------

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      // if (RemoveVoxel(voxel_data, &player, screen_width, screen_height,
      //                 MAX_PLAYER_RANGE)) {
      //   is_visibility_updatable = true;
      // }
    }

    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    UpdatePlayer(&player, p_chunk_data);
    UpdateCameraAngle(&player);
    if (is_visibility_updatable) {
      UpdateVisibility(p_chunk_data);
      CulledMeshing(p_chunk_data);
      is_visibility_updatable = false;
    }
    //----------------------------------------------------------------------------------
    // Draw 3D
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);
    StartPerformanceTracker("Render mesh");
    for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
      Matrix transform = MatrixTranslate(
          p_chunk_data[i].position.x_offset * N_VOXEL_X * VOXEL_SIZE, 0.0f,
          p_chunk_data[i].position.z_offset * N_VOXEL_Z * VOXEL_SIZE);
      DrawMesh(p_chunk_data[i].chunk_mesh, material, transform);
    }

    EndPerformanceTracker("Render mesh");

    Draw3DDebugInformation(screen_width, screen_height);
    EndMode3D();

    //----------------------------------------------------------------------------------
    // Draw 2D
    //----------------------------------------------------------------------------------
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    Draw2DDebugInformation(screen_width, screen_height);

    EndDrawing();

    EndPerformanceTracker("CompleteLoop");
  } while (!WindowShouldClose());

  //----------------------------------------------------------------------------------
  // Free rescource
  //----------------------------------------------------------------------------------
  UnloadMaterial(material);
  FreeAllChunkData(p_chunk_data);
  CloseWindow();
  PrintPerformanceTrackers();
  WritePerformanceTrackersToFile("Performance_Report.txt");
  return 0;
}

void FreeAllChunkData(Chunk *chunk_data) {
  for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
    UnloadMesh(chunk_data[i].chunk_mesh);
    free(chunk_data[i].p_voxel_data);
  }
}
void Draw3DDebugInformation(int screen_width, int screen_height) {

  StartPerformanceTracker("Draw 3D debug information");
  DrawGrid(100, VOXEL_SIZE);
  // Draw coordinate system
  DrawCylinderEx(
      (Vector3){-VOXEL_SIZE / 2, (float)N_VOXEL_Y / 2, -VOXEL_SIZE / 2},
      (Vector3){50.0f, (float)N_VOXEL_Y / 2, 0.0f}, 0.01f, 0.01f, 24, RED);
  DrawCylinderEx((Vector3){-VOXEL_SIZE / 2, 0.0f, -VOXEL_SIZE / 2},
                 (Vector3){0.0f, 50.0f, 0.0f}, 0.01f, 0.01f, 24, GREEN);
  DrawCylinderEx(
      (Vector3){-VOXEL_SIZE / 2, (float)N_VOXEL_Y / 2, -VOXEL_SIZE / 2},
      (Vector3){0.0f, (float)N_VOXEL_Y / 2, 50.0f}, 0.01f, 0.01f, 24, BLUE);
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

Chunk *InitChunks() {
  u8 i = 0;

  Chunk *chunk_data = calloc(CHUNKS_IN_TOTAL, sizeof(Chunk));

  for (u8 x = 0; x < N_CHUNKS_X; x++) {
    for (u8 z = 0; z < N_CHUNKS_Z; z++) {
      chunk_data[i].p_voxel_data = InitVoxelPointer(true);
      chunk_data[i].position =
          (s16Vector2D){x - (N_CHUNKS_X / 2), z - (N_CHUNKS_Z / 2)};
      chunk_data[i].is_mesh_dirty = true;
      i++;
    }
  }
  return chunk_data;
}
