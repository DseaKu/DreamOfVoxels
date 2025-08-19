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

// Forward declaration
Mesh GenerateGreedyMesh(Voxel *voxel_data);

int Scene3DGame() {

  const u64 screen_width = MAX_SCREEN_WIDTH;
  const u64 screen_height = MAX_SCREEN_HEIGHT;

  // INIT
  InitWindow(screen_width, screen_height, "Raylib ");
  DisableCursor();
  // SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Voxel *voxel_data = (Voxel *)calloc(NUMBER_OF_VOXELS, sizeof(Voxel));
  InitVoxel(voxel_data, true);

  UpdateVisibility(voxel_data);
  Mesh chunk_mesh = GenerateGreedyMesh(voxel_data);
  Material material = LoadMaterialDefault();
  Image texture_atlas_img = LoadImage("assets/texture_atlas.png");
  material.maps[MATERIAL_MAP_DIFFUSE].texture =
      LoadTextureFromImage(texture_atlas_img);
  UnloadImage(texture_atlas_img);

  bool is_visibility_updatable = false;
  do {
    StartPerformanceTracker("CompleteLoop");
    // Process events
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
      chunk_mesh = GenerateGreedyMesh(voxel_data);
      is_visibility_updatable = false;
    }

    // Draw 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);
    StartPerformanceTracker("Render mesh");

    DrawMesh(chunk_mesh, material, MatrixIdentity());

    EndPerformanceTracker("Render mesh");

    Draw3DDebugInformation(screen_width, screen_height);
    EndMode3D();

    // Draw 2D
    DrawCircle(screen_width / 2, screen_height / 2, CURSOR_RADIUS, BLACK);
    Draw2DDebugInformation(screen_width, screen_height);

    // End drawing
    EndDrawing();

    EndPerformanceTracker("CompleteLoop");
  } while (!WindowShouldClose());

  UnloadMaterial(material);
  UnloadMesh(chunk_mesh);
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

Mesh GenerateGreedyMesh(Voxel *voxel_data) {
  StartPerformanceTracker("GenerateGreedyMesh");
  // This is a "naive" or "culled" meshing algorithm, not a true "greedy" one.
  // It generates a quad for each visible face of a voxel.

  int max_vertices = NUMBER_OF_VOXELS * 6 * 4; // 6 faces, 4 vertices per face
  int max_indices = NUMBER_OF_VOXELS * 6 * 6;  // 6 faces, 6 indices per face

  float *vertices = (float *)malloc(max_vertices * 3 * sizeof(float));
  float *texcoords = (float *)malloc(max_vertices * 2 * sizeof(float));
  unsigned short *indices =
      (unsigned short *)malloc(max_indices * sizeof(unsigned short));

  int vertex_count = 0;
  int index_count = 0;

  for (u64 i = 0; i < NUMBER_OF_VOXELS; i++) {
    Voxel v = voxel_data[i];
    VoxelID id = (VoxelID)((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID);
    u8 visible_faces = (v >> VOXEL_SHIFT_FACE) & VOXEL_MASK_FACE;

    if (id == EMPTY || visible_faces == 0) {
      continue;
    }

    float x = (float)Voxel_GetPosX(v);
    float y = (float)Voxel_GetPosY(v);
    float z = (float)Voxel_GetPosZ(v);

    // Texture atlas settings
    const int atlas_width_in_textures = 4; // dirt, grass, stone, sand
    float texture_width = 1.0f / atlas_width_in_textures;
    float u_offset = 0.0f;

    switch (id) {
    case GRASS:
      u_offset = 0.0f * texture_width;
      break;
    case STONE:
      u_offset = 1.0f * texture_width;
      break;
    case DIRT:
      u_offset = 2.0f * texture_width;
      break;
    case SAND:
      u_offset = 3.0f * texture_width;
      break;
    default:
      break;
    }

    // Define vertices for a cube centered at origin
    float v_data[] = {// Front face
                      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
                      -0.5f, 0.5f, 0.5f,
                      // Back face
                      0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
                      -0.5f, 0.5f, 0.5f, -0.5f,
                      // Top face
                      -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
                      -0.5f, 0.5f, -0.5f,
                      // Bottom face
                      -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                      0.5f, -0.5f, -0.5f, 0.5f,
                      // Right face
                      0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
                      0.5f, 0.5f, 0.5f,
                      // Left face
                      -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
                      0.5f, -0.5f, 0.5f, -0.5f};

    // Define texture coordinates for each face
    float t_data[] = {
        u_offset,
        1.0f,
        u_offset + texture_width,
        1.0f,
        u_offset + texture_width,
        0.0f,
        u_offset,
        0.0f,
        u_offset,
        1.0f,
        u_offset + texture_width,
        1.0f,
        u_offset + texture_width,
        0.0f,
        u_offset,
        0.0f,
        u_offset,
        1.0f,
        u_offset + texture_width,
        1.0f,
        u_offset + texture_width,
        0.0f,
        u_offset,
        0.0f,
        u_offset,
        1.0f,
        u_offset + texture_width,
        1.0f,
        u_offset + texture_width,
        0.0f,
        u_offset,
        0.0f,
        u_offset,
        1.0f,
        u_offset + texture_width,
        1.0f,
        u_offset + texture_width,
        0.0f,
        u_offset,
        0.0f,
        u_offset,
        1.0f,
        u_offset + texture_width,
        1.0f,
        u_offset + texture_width,
        0.0f,
        u_offset,
        0.0f,
    };

    // Check each face
    if (visible_faces & FACE_DIR_POS_Z) { // Front
      for (int j = 0; j < 4; j++) {
        vertices[(vertex_count + j) * 3 + 0] = v_data[j * 3 + 0] + x;
        vertices[(vertex_count + j) * 3 + 1] = v_data[j * 3 + 1] + y;
        vertices[(vertex_count + j) * 3 + 2] = v_data[j * 3 + 2] + z;
        texcoords[(vertex_count + j) * 2 + 0] = t_data[j * 2 + 0];
        texcoords[(vertex_count + j) * 2 + 1] = t_data[j * 2 + 1];
      }
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 1;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count + 3;
      vertex_count += 4;
    }
    if (visible_faces & FACE_DIR_NEG_Z) { // Back
      for (int j = 0; j < 4; j++) {
        vertices[(vertex_count + j) * 3 + 0] = v_data[(j + 4) * 3 + 0] + x;
        vertices[(vertex_count + j) * 3 + 1] = v_data[(j + 4) * 3 + 1] + y;
        vertices[(vertex_count + j) * 3 + 2] = v_data[(j + 4) * 3 + 2] + z;
        texcoords[(vertex_count + j) * 2 + 0] = t_data[(j + 4) * 2 + 0];
        texcoords[(vertex_count + j) * 2 + 1] = t_data[(j + 4) * 2 + 1];
      }
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 1;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count + 3;
      vertex_count += 4;
    }
    if (visible_faces & FACE_DIR_POS_Y) { // Top
      for (int j = 0; j < 4; j++) {
        vertices[(vertex_count + j) * 3 + 0] = v_data[(j + 8) * 3 + 0] + x;
        vertices[(vertex_count + j) * 3 + 1] = v_data[(j + 8) * 3 + 1] + y;
        vertices[(vertex_count + j) * 3 + 2] = v_data[(j + 8) * 3 + 2] + z;
        texcoords[(vertex_count + j) * 2 + 0] = t_data[(j + 8) * 2 + 0];
        texcoords[(vertex_count + j) * 2 + 1] = t_data[(j + 8) * 2 + 1];
      }
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 1;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count + 3;
      vertex_count += 4;
    }
    if (visible_faces & FACE_DIR_NEG_Y) { // Bottom
      for (int j = 0; j < 4; j++) {
        vertices[(vertex_count + j) * 3 + 0] = v_data[(j + 12) * 3 + 0] + x;
        vertices[(vertex_count + j) * 3 + 1] = v_data[(j + 12) * 3 + 1] + y;
        vertices[(vertex_count + j) * 3 + 2] = v_data[(j + 12) * 3 + 2] + z;
        texcoords[(vertex_count + j) * 2 + 0] = t_data[(j + 12) * 2 + 0];
        texcoords[(vertex_count + j) * 2 + 1] = t_data[(j + 12) * 2 + 1];
      }
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 1;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count + 3;
      vertex_count += 4;
    }
    if (visible_faces & FACE_DIR_POS_X) { // Right
      for (int j = 0; j < 4; j++) {
        vertices[(vertex_count + j) * 3 + 0] = v_data[(j + 16) * 3 + 0] + x;
        vertices[(vertex_count + j) * 3 + 1] = v_data[(j + 16) * 3 + 1] + y;
        vertices[(vertex_count + j) * 3 + 2] = v_data[(j + 16) * 3 + 2] + z;
        texcoords[(vertex_count + j) * 2 + 0] = t_data[(j + 16) * 2 + 0];
        texcoords[(vertex_count + j) * 2 + 1] = t_data[(j + 16) * 2 + 1];
      }
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 1;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count + 3;
      vertex_count += 4;
    }
    if (visible_faces & FACE_DIR_NEG_X) { // Left
      for (int j = 0; j < 4; j++) {
        vertices[(vertex_count + j) * 3 + 0] = v_data[(j + 20) * 3 + 0] + x;
        vertices[(vertex_count + j) * 3 + 1] = v_data[(j + 20) * 3 + 1] + y;
        vertices[(vertex_count + j) * 3 + 2] = v_data[(j + 20) * 3 + 2] + z;
        texcoords[(vertex_count + j) * 2 + 0] = t_data[(j + 20) * 2 + 0];
        texcoords[(vertex_count + j) * 2 + 1] = t_data[(j + 20) * 2 + 1];
      }
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 1;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count;
      indices[index_count++] = vertex_count + 2;
      indices[index_count++] = vertex_count + 3;
      vertex_count += 4;
    }
  }

  Mesh mesh = {0};
  mesh.vertexCount = vertex_count;
  mesh.triangleCount = index_count / 3;
  mesh.vertices = (float *)realloc(vertices, vertex_count * 3 * sizeof(float));
  mesh.texcoords =
      (float *)realloc(texcoords, vertex_count * 2 * sizeof(float));
  mesh.indices =
      (unsigned short *)realloc(indices, index_count * sizeof(unsigned short));

  UploadMesh(&mesh, false);

  EndPerformanceTracker("GenerateGreedyMesh");
  return mesh;
}
