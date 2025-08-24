#include "scene_voxel_game.h"
#include "debug_logger.h"
#include "player.h"
#include "resource_tracker.h"
#include "voxel.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
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
  bool show_3d_debug_info = true;

  //----------------------------------------------------------------------------------
  // Game loop
  //----------------------------------------------------------------------------------
  do {
    StartPerformanceTracker("CompleteLoop");
    BeginDrawing();

    if (GetTime() > 10) {
      {
      }
    }
    //----------------------------------------------------------------------------------
    // Get and process inputs
    //----------------------------------------------------------------------------------
    if (IsKeyPressed(KEY_F1)) {
      show_3d_debug_info = !show_3d_debug_info;
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
    DrawFPS(screen_width - 100, 10);

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
void Draw2DDebugInformation(int screen_width, int screen_height,
                            Chunk *chunk_data, Player *player) {
  StartPerformanceTracker("Draw 2D debug information");

  Chunk current_chunk = chunk_data[GetIndexCurrentChunk(player)];
  DrawText("X-Axis", screen_width - 40, 30, 10, RED);
  DrawText("Y-Axis", screen_width - 40, 40, 10, GREEN);
  DrawText("Z-Axis", screen_width - 40, 50, 10, BLUE);
  DrawText(
      TextFormat("Player location\n Voxel:  x=%i z=%i y=%i\n Exact: x=%.1f "
                 "z=%.1f y=%.1f\n Chunk:: x=%d z=%d",
                 player->body.voxel_position.x, player->body.voxel_position.z,
                 player->body.voxel_position.y, player->body.position.x,
                 player->body.position.z, player->body.position.y,
                 current_chunk.position.x, current_chunk.position.z),
      screen_width - 200, 30, 10, BLACK);

  // Print debug events in the following MAROON draw text function
  DrawDebugMessages();

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
