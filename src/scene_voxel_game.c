#include "scene_voxel_game.h"
#include "debug_logger.h"
#include "player.h"
#include "resource_tracker.h"
#include "voxel.h"
#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <stdlib.h>

// Extracts the 6 planes of the camera frustum
void ExtractFrustumPlanes(float frustum[6][4]) {
  Matrix matView = rlGetMatrixModelview();
  Matrix matProj = rlGetMatrixProjection();
  Matrix clip = MatrixMultiply(matView, matProj);

  // Right plane
  frustum[0][0] = clip.m3 - clip.m0;
  frustum[0][1] = clip.m7 - clip.m4;
  frustum[0][2] = clip.m11 - clip.m8;
  frustum[0][3] = clip.m15 - clip.m12;
  // Left plane
  frustum[1][0] = clip.m3 + clip.m0;
  frustum[1][1] = clip.m7 + clip.m4;
  frustum[1][2] = clip.m11 + clip.m8;
  frustum[1][3] = clip.m15 + clip.m12;
  // Bottom plane
  frustum[2][0] = clip.m3 + clip.m1;
  frustum[2][1] = clip.m7 + clip.m5;
  frustum[2][2] = clip.m11 + clip.m9;
  frustum[2][3] = clip.m15 + clip.m13;
  // Top plane
  frustum[3][0] = clip.m3 - clip.m1;
  frustum[3][1] = clip.m7 - clip.m5;
  frustum[3][2] = clip.m11 - clip.m9;
  frustum[3][3] = clip.m15 - clip.m13;
  // Far plane
  frustum[4][0] = clip.m3 - clip.m2;
  frustum[4][1] = clip.m7 - clip.m6;
  frustum[4][2] = clip.m11 - clip.m10;
  frustum[4][3] = clip.m15 - clip.m14;
  // Near plane
  frustum[5][0] = clip.m3 + clip.m2;
  frustum[5][1] = clip.m7 + clip.m6;
  frustum[5][2] = clip.m11 + clip.m10;
  frustum[5][3] = clip.m15 + clip.m14;

  // Normalize planes
  for (int i = 0; i < 6; i++) {
    float length =
        sqrtf(frustum[i][0] * frustum[i][0] + frustum[i][1] * frustum[i][1] +
              frustum[i][2] * frustum[i][2]);
    frustum[i][0] /= length;
    frustum[i][1] /= length;
    frustum[i][2] /= length;
    frustum[i][3] /= length;
  }
}

// Check if a bounding box is inside the frustum
bool IsBoxInFrustum(float frustum[6][4], BoundingBox box) {
  for (int i = 0; i < 6; i++) {
    // Find the vertex of the box that is furthest in the direction of the
    // plane's normal
    Vector3 p_vertex = {box.min.x, box.min.y, box.min.z};
    if (frustum[i][0] > 0)
      p_vertex.x = box.max.x;
    if (frustum[i][1] > 0)
      p_vertex.y = box.max.y;
    if (frustum[i][2] > 0)
      p_vertex.z = box.max.z;

    // Check if this vertex is outside the plane
    if (frustum[i][0] * p_vertex.x + frustum[i][1] * p_vertex.y +
            frustum[i][2] * p_vertex.z + frustum[i][3] <
        0) {
      return false; // The entire box is outside this plane
    }
  }
  return true; // Box is intersecting or inside the frustum
}

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

    float frustum[6][4];
    ExtractFrustumPlanes(frustum);
    int chunks_drawn = 0;

    StartPerformanceTracker("Render mesh");
    for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
      float chunk_world_x =
          (float)chunk_data[i].position.x * N_VOXEL_X * VOXEL_SIZE;
      float chunk_world_z =
          (float)chunk_data[i].position.z * N_VOXEL_Z * VOXEL_SIZE;

      BoundingBox chunk_bbox = {
          .min = {chunk_world_x, 0.0f, chunk_world_z},
          .max = {chunk_world_x + N_VOXEL_X * VOXEL_SIZE,
                  N_VOXEL_Y * VOXEL_SIZE,
                  chunk_world_z + N_VOXEL_Z * VOXEL_SIZE}};

      if (IsBoxInFrustum(frustum, chunk_bbox)) {
        Matrix transform = MatrixTranslate(chunk_world_x, 0.0f, chunk_world_z);
        DrawMesh(chunk_data[i].chunk_mesh, material, transform);
        chunks_drawn++;
      }
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
    Draw2DDebugInformation(screen_width, screen_height, chunk_data, &player,
                           chunks_drawn);

    EndDrawing();

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
                            Chunk *chunk_data, Player *player,
                            int chunks_drawn) {
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

  DrawText(TextFormat("Chunks drawn: %d/%d", chunks_drawn, CHUNKS_IN_TOTAL),
           screen_width - 200, 100, 10, MAROON);

  // Print debug events in the following MAROON draw text function
  DrawDebugMessages();
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
