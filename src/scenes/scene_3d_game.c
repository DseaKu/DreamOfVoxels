#include "scenes/scene_3d_game.h"
#include "core/voxel.h"
#include "rlgl.h"
#include "sprites/player.h"
#include "utils/resource_tracker.h"
#include <float.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // For memset

int Scene3DGame() {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  //----------------------------------------------------------------------------------
  // Init
  //----------------------------------------------------------------------------------
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  // SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Voxel *voxel_data = (Voxel *)calloc(NUMBER_OF_VOXELS, sizeof(Voxel));
  InitVoxel(voxel_data, true);

  UpdateVisibility(voxel_data);
  Mesh chunk_mesh = CulledMeshing(voxel_data);
  Material material = LoadMaterialDefault();
  Image texture_atlas_img = LoadImage("assets/texture_atlas.png");
  material.maps[MATERIAL_MAP_DIFFUSE].texture =
      LoadTextureFromImage(texture_atlas_img);
  UnloadImage(texture_atlas_img);

  bool is_visibility_updatable = false;
  UpdateCameraAngle(&player); // Update camera parameters

  //----------------------------------------------------------------------------------
  // Game loop
  //----------------------------------------------------------------------------------
  do {
    StartPerformanceTracker("CompleteLoop");
    //----------------------------------------------------------------------------------
    // Process inputs
    //----------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------
    // Process events
    //----------------------------------------------------------------------------------
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      if (RemoveVoxel(voxel_data, &player, screen_width, screen_height,
                      MAX_PLAYER_RANGE)) {
        is_visibility_updatable = true;
      }
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
      UnloadMesh(chunk_mesh);
      chunk_mesh = CulledMeshing(voxel_data);
      is_visibility_updatable = false;
    }

    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    Vector2 mouse_delta = GetMouseDelta();
    lookRotation.x -= mouse_delta.x * sensitivity.x;
    lookRotation.y += mouse_delta.y * sensitivity.y;

    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
    UpdateBody(&player.body, lookRotation.x, sideway, forward,
               IsKeyPressed(KEY_SPACE), crouching);

    float delta = GetFrameTime();
    headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT),
                    20.0f * delta);
    player.camera.position = (Vector3){
        player.body.position.x,
        player.body.position.y + (BOTTOM_HEIGHT + headLerp),
        player.body.position.z,
    };

    if (player.body.isGrounded && ((forward != 0) || (sideway != 0))) {
      headTimer += delta * 3.0f;
      walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
      player.camera.fovy = Lerp(player.camera.fovy, 55.0f, 5.0f * delta);
    } else {
      walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
      player.camera.fovy = Lerp(player.camera.fovy, 60.0f, 5.0f * delta);
    }

    lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
    lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);

    UpdateCameraAngle(&player);

    //----------------------------------------------------------------------------------
    // Draw 3D
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);
    StartPerformanceTracker("Render mesh");

    DrawMesh(chunk_mesh, material, MatrixIdentity());

    EndPerformanceTracker("Render mesh");

    Draw3DDebugInformation(screen_width, screen_height);
    EndMode3D();

    //----------------------------------------------------------------------------------
    // Draw 2D
    //----------------------------------------------------------------------------------
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    Draw2DDebugInformation(screen_width, screen_height);

    //----------------------------------------------------------------------------------
    // End Drawing
    //----------------------------------------------------------------------------------
    EndDrawing();

    EndPerformanceTracker("CompleteLoop");
  } while (!WindowShouldClose());

  UnloadMaterial(material);
  UnloadMesh(chunk_mesh);
  free(voxel_data);
  CloseWindow();
  PrintPerformanceTrackers();
  WritePerformanceTrackersToFile("Performance_Report.txt");
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
