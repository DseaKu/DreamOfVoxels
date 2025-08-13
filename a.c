
/*******************************************************************************************
 *
 *   raylib [models] example - Cubicmap loading and drawing
 *
 *   Example complexity rating: [★★☆☆] 2/4
 *
 *   Example originally created with raylib 1.8, last time updated with
 *raylib 3.5
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2015-2025 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/
#include "raylib.h"

int main2(void) {
  // Initialization
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight,
             "raylib [models] - Minecraft-like block");

  // Define the camera to look into our 3d world
  Camera camera = {0};
  camera.position = (Vector3){5.0f, 5.0f, 5.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // Load the texture atlas
  Texture2D texture = LoadTexture("log_oak_top_2.png");

  // Use a standard cube mesh
  Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  Model model = LoadModelFromMesh(mesh);

  // Apply the texture to the model
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_ORBITAL);

    // Begin Drawing
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // CORRECTED LINE: Explicitly cast the position to Vector3
    DrawModel(model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
    DrawGrid(10, 1.0f);

    EndMode3D();

    DrawText("This is a cube with a texture atlas.", 10, 40, 20, DARKGRAY);
    DrawText("To have different faces, a custom mesh is required.", 10, 65, 20,
             DARKGRAY);
    DrawFPS(10, 10);

    EndDrawing();
  }

  // Unload resources
  UnloadTexture(texture);
  UnloadModel(model);

  CloseWindow();

  return 0;
}
