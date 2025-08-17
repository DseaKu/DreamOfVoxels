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
  SetTargetFPS(TARGET_FPS);
  Player player = InitPlayer();
  Voxel *voxel_data = (Voxel *)calloc(NUMBER_OF_VOXELS, sizeof(Voxel));
  InitVoxel(voxel_data);

  UpdateVisibility(voxel_data);

  Texture2D dirt_texture = LoadTexture("assets/log.png");
  Material material = LoadMaterialDefault();
  material.maps[MATERIAL_MAP_DIFFUSE].texture = dirt_texture;

  Mesh chunk_mesh = GenerateGreedyMesh(voxel_data);

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
      UnloadMesh(chunk_mesh);                      // Unload old mesh
      chunk_mesh = GenerateGreedyMesh(voxel_data); // Regenerate mesh
      is_visibility_updatable = false;
    }

    // Draw 3D
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(player.camera);

    DrawMesh(chunk_mesh, material, MatrixIdentity());
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

// A fairly standard greedy meshing implementation.
Mesh GenerateGreedyMesh(Voxel *voxel_data) {
  StartPerformanceTracker("GenerateGreedyMesh");

  // Dynamic arrays for mesh data
  Vector3 *vertices = NULL;
  Vector3 *normals = NULL;
  Vector2 *texcoords = NULL;
  int face_count = 0;

  // Iterate over the 3 axes
  for (int axis = 0; axis < 3; ++axis) {
    int u = (axis + 1) % 3;
    int v = (axis + 2) % 3;

    int x[3] = {0};
    int q[3] = {0};
    q[axis] = 1;

    int dims[2] = {0, 0};
    if (axis == 0) {
      dims[0] = Y_MAX;
      dims[1] = Z_MAX;
    } else if (axis == 1) {
      dims[0] = X_MAX;
      dims[1] = Z_MAX;
    } else {
      dims[0] = X_MAX;
      dims[1] = Y_MAX;
    }

    VoxelID *mask = (VoxelID *)calloc(dims[0] * dims[1], sizeof(VoxelID));

    // Iterate over each slice of the chunk
    for (x[axis] = -1;
         x[axis] < (axis == 0 ? X_MAX : (axis == 1 ? Y_MAX : Z_MAX));) {
      int n = 0;
      for (x[v] = 0; x[v] < dims[0]; ++x[v]) {
        for (x[u] = 0; x[u] < dims[1]; ++x[u]) {
          VoxelID id1 =
              (x[axis] >= 0) ? GetVoxelID(voxel_data, x[0], x[1], x[2]) : EMPTY;
          VoxelID id2 =
              (x[axis] < (axis == 0 ? X_MAX : (axis == 1 ? Y_MAX : Z_MAX)) - 1)
                  ? GetVoxelID(voxel_data, x[0] + q[0], x[1] + q[1],
                               x[2] + q[2])
                  : EMPTY;

          mask[n++] = (id1 != EMPTY && id2 == EMPTY)   ? id1
                      : (id1 == EMPTY && id2 != EMPTY) ? id2
                                                       : 0;
        }
      }

      x[axis]++;
      n = 0;

      for (int j = 0; j < dims[0]; ++j) {
        for (int i = 0; i < dims[1];) {
          if (mask[n]) {
            int w, h;
            for (w = 1; i + w < dims[1] && mask[n + w] == mask[n]; ++w) {
            }

            bool done = false;
            for (h = 1; j + h < dims[0]; ++h) {
              for (int k = 0; k < w; ++k) {
                if (mask[n + k + h * dims[1]] != mask[n]) {
                  done = true;
                  break;
                }
              }
              if (done)
                break;
            }

            x[u] = i;
            x[v] = j;

            int du[3] = {0};
            du[u] = w;

            int dv[3] = {0};
            dv[v] = h;

            face_count++;
            vertices =
                (Vector3 *)realloc(vertices, face_count * 4 * sizeof(Vector3));
            normals =
                (Vector3 *)realloc(normals, face_count * 4 * sizeof(Vector3));
            texcoords =
                (Vector2 *)realloc(texcoords, face_count * 4 * sizeof(Vector2));

            Vector3 v1 = {x[0], x[1], x[2]};
            Vector3 v2 = {x[0] + du[0], x[1] + du[1], x[2] + du[2]};
            Vector3 v3 = {x[0] + du[0] + dv[0], x[1] + du[1] + dv[1],
                          x[2] + du[2] + dv[2]};
            Vector3 v4 = {x[0] + dv[0], x[1] + dv[1], x[2] + dv[2]};

            int vert_idx = (face_count - 1) * 4;
            vertices[vert_idx + 0] = v1;
            vertices[vert_idx + 1] = v2;
            vertices[vert_idx + 2] = v3;
            vertices[vert_idx + 3] = v4;

            texcoords[vert_idx + 0] = (Vector2){0, 0};
            texcoords[vert_idx + 1] = (Vector2){(float)w, 0};
            texcoords[vert_idx + 2] = (Vector2){(float)w, (float)h};
            texcoords[vert_idx + 3] = (Vector2){0, (float)h};

            Vector3 norm = {q[0], q[1], q[2]};
            normals[vert_idx + 0] = norm;
            normals[vert_idx + 1] = norm;
            normals[vert_idx + 2] = norm;
            normals[vert_idx + 3] = norm;

            for (int l = 0; l < h; ++l) {
              for (int k = 0; k < w; ++k) {
                mask[n + k + l * dims[1]] = 0;
              }
            }
            i += w;
            n += w;
          } else {
            i++;
            n++;
          }
        }
      }
    }
    free(mask);
  }

  Mesh mesh = {0};
  mesh.vertexCount = face_count * 4;
  mesh.triangleCount = face_count * 2;
  mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
  mesh.normals = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
  mesh.texcoords = (float *)malloc(mesh.vertexCount * 2 * sizeof(float));
  mesh.indices =
      (unsigned short *)malloc(mesh.triangleCount * 3 * sizeof(unsigned short));

  int vert_idx = 0;
  int ind_idx = 0;
  for (int i = 0; i < face_count; ++i) {
    for (int j = 0; j < 4; ++j) {
      int v_idx = i * 4 + j;
      mesh.vertices[vert_idx * 3 + 0] = vertices[v_idx].x - 0.5f;
      mesh.vertices[vert_idx * 3 + 1] = vertices[v_idx].y - 0.5f;
      mesh.vertices[vert_idx * 3 + 2] = vertices[v_idx].z - 0.5f;
      mesh.normals[vert_idx * 3 + 0] = normals[v_idx].x;
      mesh.normals[vert_idx * 3 + 1] = normals[v_idx].y;
      mesh.normals[vert_idx * 3 + 2] = normals[v_idx].z;
      mesh.texcoords[vert_idx * 2 + 0] = texcoords[v_idx].x;
      mesh.texcoords[vert_idx * 2 + 1] = texcoords[v_idx].y;
      vert_idx++;
    }
    int base_idx = i * 4;
    mesh.indices[ind_idx++] = base_idx + 0;
    mesh.indices[ind_idx++] = base_idx + 1;
    mesh.indices[ind_idx++] = base_idx + 2;
    mesh.indices[ind_idx++] = base_idx + 0;
    mesh.indices[ind_idx++] = base_idx + 2;
    mesh.indices[ind_idx++] = base_idx + 3;
  }

  free(vertices);
  free(normals);
  free(texcoords);

  UploadMesh(&mesh, false);
  EndPerformanceTracker("GenerateGreedyMesh");
  return mesh;
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
