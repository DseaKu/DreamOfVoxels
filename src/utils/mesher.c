#include "utils/mesher.h"
#include "scenes/scene_3d_game.h"
#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>
#include <string.h>

// Helper function to get block state, returns false for out-of-bounds
static bool IsBlockActive(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX], int x,
    int y, int z) {
  if (x < 0 || x >= PLAYGROUND_X_MAX || y < 0 || y >= PLAYGROUND_Y_MAX ||
      z < 0 || z >= PLAYGROUND_Z_MAX) {
    return false;
  }
  return blocks[x][y][z].is_active;
}

// Helper function to add a quad to the mesh data
static void AddQuad(Vector3 *vertices, Vector2 *texcoords, Vector3 *normals,
                    int *v_count, bool is_back_face, Vector3 v1, Vector3 v2,
                    Vector3 v3, Vector3 v4, Vector2 t1, Vector2 t2,
                    Vector3 normal) {
  int base = *v_count;

  if (is_back_face) {
    // Triangle 1
    vertices[base] = v1;
    vertices[base + 1] = v3;
    vertices[base + 2] = v2;
    texcoords[base] = (Vector2){t1.x, t1.y};
    texcoords[base + 1] = (Vector2){t1.x, t2.y};
    texcoords[base + 2] = (Vector2){t2.x, t1.y};
    normals[base] = normal;
    normals[base + 1] = normal;
    normals[base + 2] = normal;

    // Triangle 2
    vertices[base + 3] = v2;
    vertices[base + 4] = v3;
    vertices[base + 5] = v4;
    texcoords[base + 3] = (Vector2){t2.x, t1.y};
    texcoords[base + 4] = (Vector2){t1.x, t2.y};
    texcoords[base + 5] = (Vector2){t2.x, t2.y};
    normals[base + 3] = normal;
    normals[base + 4] = normal;
    normals[base + 5] = normal;
  } else {
    // Triangle 1
    vertices[base] = v1;
    vertices[base + 1] = v2;
    vertices[base + 2] = v3;
    texcoords[base] = (Vector2){t1.x, t1.y};
    texcoords[base + 1] = (Vector2){t2.x, t1.y};
    texcoords[base + 2] = (Vector2){t1.x, t2.y};
    normals[base] = normal;
    normals[base + 1] = normal;
    normals[base + 2] = normal;

    // Triangle 2
    vertices[base + 3] = v3;
    vertices[base + 4] = v2;
    vertices[base + 5] = v4;
    texcoords[base + 3] = (Vector2){t1.x, t2.y};
    texcoords[base + 4] = (Vector2){t2.x, t1.y};
    texcoords[base + 5] = (Vector2){t2.x, t2.y};
    normals[base + 3] = normal;
    normals[base + 4] = normal;
    normals[base + 5] = normal;
  }

  *v_count += 6;
}

Mesh GenerateGreedyMesh(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]) {
  int max_vertices =
      PLAYGROUND_X_MAX * PLAYGROUND_Y_MAX * PLAYGROUND_Z_MAX * 18;
  Vector3 *vertices = (Vector3 *)malloc(max_vertices * sizeof(Vector3));
  Vector2 *texcoords = (Vector2 *)malloc(max_vertices * sizeof(Vector2));
  Vector3 *normals = (Vector3 *)malloc(max_vertices * sizeof(Vector3));
  int v_count = 0;

  const int dims[3] = {PLAYGROUND_X_MAX, PLAYGROUND_Y_MAX, PLAYGROUND_Z_MAX};

  for (int d = 0; d < 3; ++d) {
    int u = (d + 1) % 3;
    int v = (d + 2) % 3;

    const int slice_dims[2] = {dims[u], dims[v]};
    bool *mask = (bool *)malloc(slice_dims[0] * slice_dims[1] * sizeof(bool));
    int x[3] = {0};

    for (x[d] = -1; x[d] < dims[d]; ++x[d]) {
      int n = 0;
      for (x[v] = 0; x[v] < slice_dims[1]; ++x[v]) {
        for (x[u] = 0; x[u] < slice_dims[0]; ++x[u]) {
          bool block_current = IsBlockActive(blocks, x[0], x[1], x[2]);
          int next_x[3] = {x[0], x[1], x[2]};
          next_x[d]++;
          bool block_neighbor =
              IsBlockActive(blocks, next_x[0], next_x[1], next_x[2]);
          mask[n++] = block_current != block_neighbor;
        }
      }

      n = 0;
      for (int j = 0; j < slice_dims[1]; ++j) {
        for (int i = 0; i < slice_dims[0]; ++i) {
          if (mask[n]) {
            int w, h;
            for (w = 1; i + w < slice_dims[0] && mask[n + w]; ++w) {
            }

            bool done = false;
            for (h = 1; j + h < slice_dims[1]; ++h) {
              for (int k = 0; k < w; ++k) {
                if (!mask[n + k + h * slice_dims[0]]) {
                  done = true;
                  break;
                }
              }
              if (done)
                break;
            }

            x[u] = i;
            x[v] = j;

            int next_x[3] = {x[0], x[1], x[2]};
            next_x[d]++;
            bool is_back_face =
                IsBlockActive(blocks, next_x[0], next_x[1], next_x[2]);

            float du[3] = {0};
            du[u] = w;
            float dv[3] = {0};
            dv[v] = h;

            int pos[3] = {x[0], x[1], x[2]};
            pos[d]++;

            Vector3 v1 = {(float)pos[0], (float)pos[1], (float)pos[2]};
            Vector3 v2 = {(float)pos[0] + du[0], (float)pos[1] + du[1],
                          (float)pos[2] + du[2]};
            Vector3 v3 = {(float)pos[0] + dv[0], (float)pos[1] + dv[1],
                          (float)pos[2] + dv[2]};
            Vector3 v4 = {(float)pos[0] + du[0] + dv[0],
                          (float)pos[1] + du[1] + dv[1],
                          (float)pos[2] + du[2] + dv[2]};

            Vector2 t1 = {0.0f, 0.0f};
            Vector2 t2 = {(float)w, (float)h};

            float normal_dir[3] = {0};
            normal_dir[d] = is_back_face ? -1.0f : 1.0f;
            Vector3 normal = {normal_dir[0], normal_dir[1], normal_dir[2]};

            AddQuad(vertices, texcoords, normals, &v_count, is_back_face, v1,
                    v2, v3, v4, t1, t2, normal);

            for (int l = 0; l < h; ++l) {
              for (int k = 0; k < w; ++k) {
                mask[n + k + l * slice_dims[0]] = false;
              }
            }
            i += w - 1;
          }
          n++;
        }
      }
    }
    free(mask);
  }

  Mesh mesh = {0};
  mesh.vertexCount = v_count;
  mesh.triangleCount = v_count / 3;
  mesh.vertices = (float *)malloc(v_count * 3 * sizeof(float));
  mesh.texcoords = (float *)malloc(v_count * 2 * sizeof(float));
  mesh.normals = (float *)malloc(v_count * 3 * sizeof(float));

  for (int i = 0; i < v_count; i++) {
    mesh.vertices[i * 3] = vertices[i].x;
    mesh.vertices[i * 3 + 1] = vertices[i].y;
    mesh.vertices[i * 3 + 2] = vertices[i].z;
    mesh.texcoords[i * 2] = texcoords[i].x;
    mesh.texcoords[i * 2 + 1] = texcoords[i].y;
    mesh.normals[i * 3] = normals[i].x;
    mesh.normals[i * 3 + 1] = normals[i].y;
    mesh.normals[i * 3 + 2] = normals[i].z;
  }

  free(vertices);
  free(texcoords);
  free(normals);

  UploadMesh(&mesh, false);

  return mesh;
}
