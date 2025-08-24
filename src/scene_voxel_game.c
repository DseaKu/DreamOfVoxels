#include "scene_voxel_game.h"
#include "chunk.h"
#include "debug_logger.h"
#include "player.h"
#include "resource_tracker.h"
#include "voxel.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdbool.h>
#include <stdlib.h>

int Scene3DGame() {

  //----------------------------------------------------------------------------------
  // Init
  //----------------------------------------------------------------------------------
  const u64 screen_width = SCREEN_WIDTH;
  const u64 screen_height = SCREEN_HEIGHT;
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();

  // SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Chunk *chunk_data = InitChunks();

  Material material = LoadMaterialDefault();
  Image texture_atlas_img = LoadImage("assets/texture_atlas.png");
  material.maps[MATERIAL_MAP_DIFFUSE].texture =
      LoadTextureFromImage(texture_atlas_img);
  UnloadImage(texture_atlas_img);

  bool is_visibility_updatable = true;
  bool show_3d_debug_info = false;
  bool show_fps_info = true;

  //----------------------------------------------------------------------------------
  // Game loop
  //----------------------------------------------------------------------------------
  do {
    StartPerformanceTracker("CompleteLoop");
    BeginDrawing();

    if (GetTime() > 5) {
      {
        s8 a = 0;
      }
    }
    //----------------------------------------------------------------------------------
    // Get and process inputs
    //----------------------------------------------------------------------------------
    if (IsKeyPressed(KEY_F1) || IsKeyPressed(KEY_G)) {
      show_3d_debug_info = !show_3d_debug_info;
    }
    if (IsKeyPressed(KEY_F2)) {
      show_fps_info = !show_fps_info;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      // if (RemoveVoxel(voxel_data, &player, screen_width, screen_height,
      //                 MAX_PLAYER_RANGE)) {
      //   is_visibility_updatable = true;
      // }
    }

    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    UpdatePlayer(&player, chunk_data);
    UpdateCameraAngle(&player);
    if (is_visibility_updatable) {
      UpdateVisibility(chunk_data);
      CulledMeshing(chunk_data);
      is_visibility_updatable = false;
    }
    //----------------------------------------------------------------------------------
    // Draw 3D
    //----------------------------------------------------------------------------------
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    StartPerformanceTracker("Render mesh");
    for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
      Matrix transform = MatrixTranslate(
          chunk_data[i].position.x * N_VOXEL_X * VOXEL_SIZE, 0.0f,
          chunk_data[i].position.z * N_VOXEL_Z * VOXEL_SIZE);
      DrawMesh(chunk_data[i].chunk_mesh, material, transform);
    }

    EndPerformanceTracker("Render mesh");

    if (show_3d_debug_info) {
      Draw3DDebugInformation(screen_width, screen_height);
    }
    EndMode3D();

    //----------------------------------------------------------------------------------
    // Draw 2D
    //----------------------------------------------------------------------------------
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    if (show_3d_debug_info) {
      Draw2DDebugInformation(screen_width, screen_height, chunk_data, &player);
    }
    if (show_fps_info) {
      DrawFPS(screen_width - 100, 10);
    }

    StartPerformanceTracker("EndDrawing");
    EndDrawing();
    EndPerformanceTracker("EndDrawing");

    EndPerformanceTracker("CompleteLoop");
  } while (!WindowShouldClose());

  //----------------------------------------------------------------------------------
  // Free rescource
  //----------------------------------------------------------------------------------
  UnloadMaterial(material);
  FreeAllChunkData(chunk_data);
  CloseWindow();
  PrintPerformanceTrackers();
  WritePerformanceTrackersToFile("Performance_Report.txt");
  return 0;
}
