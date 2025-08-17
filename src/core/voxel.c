#include "core/voxel.h"
#include "rlgl.h"
#include "std_includes.h"
#include "utils/resource_tracker.h"
#include <math.h> // For roundf
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

Mesh BuildSingelVoxelMesh() {
  Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  return mesh;
}

// Init xzy-coordinates to voxels
void InitVoxel(Voxel *voxel_data, bool is_random) {
  // 0.527000 ms (avg over 1 runs):Init Voxels
  StartPerformanceTracker("Init Voxels");
  u64 index = 0;

  for (u8 y = 0; y < Y_MAX; y++) {
    for (u8 z = 0; z < Z_MAX; z++) {
      for (u8 x = 0; x < X_MAX; x++) {

        Voxel v = 0;

        // Pack the data using the named shift constants
        v |= ((Voxel)x << VOXEL_SHIFT_POS_X);
        v |= ((Voxel)y << VOXEL_SHIFT_POS_Y);
        v |= ((Voxel)z << VOXEL_SHIFT_POS_Z);

        if (!is_random) {
          v |= ((Voxel)DIRT << VOXEL_SHIFT_ID);
        } else {
          // Set random voxel ids
        }

        voxel_data[index] = v;
        index++;
      }
    }
  }
  EndPerformanceTracker("Init Voxels");
}

// Gets the X position from a voxel
u8 Voxel_GetPosX(Voxel v) {
  return (u8)((v >> VOXEL_SHIFT_POS_X) & VOXEL_MASK_POS);
}

// Gets the Y position from a voxel
u8 Voxel_GetPosY(Voxel v) {
  return (u8)((v >> VOXEL_SHIFT_POS_Y) & VOXEL_MASK_POS);
}

// Gets the Z position from a voxel
u8 Voxel_GetPosZ(Voxel v) {
  return (u8)((v >> VOXEL_SHIFT_POS_Z) & VOXEL_MASK_POS);
}

void UpdateVisibility(Voxel *voxel_data) {
  /* 1.282267 ms (avg over 5952 runs):Update visibilty
  // After boundary check
  // 0.806000 ms (avg over 1 runs):Update visibilty
  */

  StartPerformanceTracker("Update visibilty");

  u64 index = 0;
  for (u8 y = 0; y < Y_MAX; y++) {
    for (u8 z = 0; z < Z_MAX; z++) {
      for (u8 x = 0; x < X_MAX; x++) {

        Voxel v = voxel_data[index];
        u8 visible_faces = 0;

        // Skip EMPTY voxel
        if (((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID) == EMPTY) {
          index++;
          continue;
        }

        // For voxels not on the boundary, skip boundary checks.
        if (x > 0 && x < X_MAX - 1 && y > 0 && y < Y_MAX - 1 && z > 0 &&
            z < Z_MAX - 1) {
          if (((voxel_data[index + X_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY)
            visible_faces |= FACE_DIR_POS_X;
          if (((voxel_data[index - X_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY)
            visible_faces |= FACE_DIR_NEG_X;
          if (((voxel_data[index + Z_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY)
            visible_faces |= FACE_DIR_POS_Z;
          if (((voxel_data[index - Z_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY)
            visible_faces |= FACE_DIR_NEG_Z;
          if (((voxel_data[index + Y_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY)
            visible_faces |= FACE_DIR_POS_Y;
          if (((voxel_data[index - Y_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY)
            visible_faces |= FACE_DIR_NEG_Y;
        } else {

          //  Boundary Voxel Check
          if (x == X_MAX - 1 ||
              ((voxel_data[index + X_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY) {
            visible_faces |= FACE_DIR_POS_X;
          }
          if (x == 0 ||
              ((voxel_data[index - X_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY) {
            visible_faces |= FACE_DIR_NEG_X;
          }
          if (z == Z_MAX - 1 ||
              ((voxel_data[index + Z_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY) {
            visible_faces |= FACE_DIR_POS_Z;
          }
          if (z == 0 ||
              ((voxel_data[index - Z_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY) {
            visible_faces |= FACE_DIR_NEG_Z;
          }
          if (y == Y_MAX - 1 ||
              ((voxel_data[index + Y_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY) {
            visible_faces |= FACE_DIR_POS_Y;
          }
          if (y == 0 ||
              ((voxel_data[index - Y_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
               VOXEL_MASK_ID) == EMPTY) {
            visible_faces |= FACE_DIR_NEG_Y;
          }
        }

        voxel_data[index] |= visible_faces << VOXEL_SHIFT_FACE;
        index++;
      }
    }
  }

  EndPerformanceTracker("Update visibilty");
}

// Generate a simple triangle mesh from code
Mesh GenMeshCustom(Vector3 offset) {
  // Initialize a mesh object
  Mesh mesh = {0};
  // Define the number of triangles in the mesh
  mesh.triangleCount = 1;
  // Calculate the number of vertices based on the triangle count
  mesh.vertexCount = mesh.triangleCount * 3;
  // Allocate memory for the vertices (3 coordinates each: x, y, z)
  mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
  // Allocate memory for the texture coordinates (2 coordinates each: x, y)
  mesh.texcoords = (float *)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
  // Allocate memory for the normals (3 coordinates each: x, y, z)
  mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

  // --- Define the vertices of the triangle ---

  // Vertex 1: Position (0, 0, 0)
  mesh.vertices[0] = 0 + offset.x;
  mesh.vertices[1] = 0 + offset.y;
  mesh.vertices[2] = 0 + offset.z;
  // Normal vector for Vertex 1 (pointing upwards)
  mesh.normals[0] = 0;
  mesh.normals[1] = 1;
  mesh.normals[2] = 0;
  // Texture coordinates for Vertex 1
  mesh.texcoords[0] = 0;
  mesh.texcoords[1] = 0;

  // Vertex 2: Position (1, 0, 2)
  mesh.vertices[3] = 1 + offset.x;
  mesh.vertices[4] = 0 + offset.y;
  mesh.vertices[5] = 2 + offset.z;
  // Normal vector for Vertex 2 (pointing upwards)
  mesh.normals[3] = 0;
  mesh.normals[4] = 1;
  mesh.normals[5] = 0;
  // Texture coordinates for Vertex 2
  mesh.texcoords[2] = 0.5f;
  mesh.texcoords[3] = 1.0f;

  // Vertex 3: Position (2, 0, 0)
  mesh.vertices[6] = 2;
  mesh.vertices[7] = 0;
  mesh.vertices[8] = 0;
  // Normal vector for Vertex 3 (pointing upwards)
  mesh.normals[6] = 0;
  mesh.normals[7] = 1;
  mesh.normals[8] = 0;
  // Texture coordinates for Vertex 3
  mesh.texcoords[4] = 1;
  mesh.texcoords[5] = 0;

  // Upload the mesh data from CPU (RAM) to GPU (VRAM)
  // The 'false' argument indicates that the mesh is not dynamic and won't be
  // updated frequently
  UploadMesh(&mesh, false);

  // Return the generated mesh
  return mesh;
}

u64 GetVoxelIndex(int x, int y, int z) {
  if (x < 0 || x >= X_MAX || y < 0 || y >= Y_MAX || z < 0 || z >= Z_MAX) {
    return -1; // Invalid index
  }
  return (y * Y_NEIGHBOUR_OFFSET) + (z * Z_NEIGHBOUR_OFFSET) + x;
}

VoxelID GetVoxelID(Voxel *voxel_data, int x, int y, int z) {
  u64 index = GetVoxelIndex(x, y, z);
  if (index == -1) {
    return EMPTY; // Out of bounds is considered empty
  }
  return (VoxelID)((voxel_data[index] >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID);
}

void PlaceVoxel(Voxel *voxel_data, int x, int y, int z, VoxelID id) {
  u64 index = GetVoxelIndex(x, y, z);
  if (index == -1) {
    return;
  }

  // Clear the old ID and set the new one
  voxel_data[index] &= ~((Voxel)VOXEL_MASK_ID << VOXEL_SHIFT_ID);
  voxel_data[index] |= ((Voxel)id << VOXEL_SHIFT_ID);
}

bool RemoveVoxel(Voxel *voxel_data, Player *player, u64 screen_width,
                 u64 screen_height, float player_range) {

  StartPerformanceTracker("Remove Voxel");
  // Raycasting to find target voxel
  Ray ray = GetMouseRay((Vector2){screen_width / 2.0f, screen_height / 2.0f},
                        player->camera);
  RayCollision closest_hit = {0};
  closest_hit.distance = player_range;
  closest_hit.hit = false;

  int hit_voxel_x = -1, hit_voxel_y = -1, hit_voxel_z = -1;

  // Iterate over a reasonable distance from the player
  for (u64 index = 0; index < NUMBER_OF_VOXELS; index++) {
    Voxel v = voxel_data[index];
    if (((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID) == EMPTY) {
      continue;
    }

    int x = Voxel_GetPosX(v);
    int y = Voxel_GetPosY(v);
    int z = Voxel_GetPosZ(v);

    BoundingBox box = {.min = (Vector3){x - 0.5f, y - 0.5f, z - 0.5f},
                       .max = (Vector3){x + 0.5f, y + 0.5f, z + 0.5f}};

    RayCollision hit = GetRayCollisionBox(ray, box);

    if (hit.hit && hit.distance < closest_hit.distance) {
      closest_hit = hit;
      hit_voxel_x = x;
      hit_voxel_y = y;
      hit_voxel_z = z;
    }
  }

  if (closest_hit.hit) {

    PlaceVoxel(voxel_data, hit_voxel_x, hit_voxel_y, hit_voxel_z, EMPTY);
  }
  EndPerformanceTracker("Remove Voxel");

  return closest_hit.hit;
}

void TryPlaceVoxel(Voxel *voxel_data, Player *player, u64 screen_width,
                   u64 screen_height, float player_range) {
  StartPerformanceTracker("TryPlaceVoxel");
  // Raycasting to find target voxel
  Ray ray = GetMouseRay((Vector2){screen_width / 2.0f, screen_height / 2.0f},
                        player->camera);
  RayCollision closest_hit = {0};
  closest_hit.distance = player_range;
  closest_hit.hit = false;

  int hit_voxel_x = -1, hit_voxel_y = -1, hit_voxel_z = -1;

  // Iterate over a reasonable distance from the player
  for (u64 index = 0; index < NUMBER_OF_VOXELS; index++) {
    Voxel v = voxel_data[index];
    if (((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID) == EMPTY) {
      continue;
    }

    int x = Voxel_GetPosX(v);
    int y = Voxel_GetPosY(v);
    int z = Voxel_GetPosZ(v);

    BoundingBox box = {.min = (Vector3){x - 0.5f, y - 0.5f, z - 0.5f},
                       .max = (Vector3){x + 0.5f, y + 0.5f, z + 0.5f}};

    RayCollision hit = GetRayCollisionBox(ray, box);

    if (hit.hit && hit.distance < closest_hit.distance) {
      closest_hit = hit;
      hit_voxel_x = x;
      hit_voxel_y = y;
      hit_voxel_z = z;
    }
  }

  if (closest_hit.hit) {
    int new_x = hit_voxel_x + (int)roundf(closest_hit.normal.x);
    int new_y = hit_voxel_y + (int)roundf(closest_hit.normal.y);
    int new_z = hit_voxel_z + (int)roundf(closest_hit.normal.z);

    // --- Player Collision Check ---
    int player_head_x = (int)roundf(player->camera.position.x);
    int player_head_y = (int)roundf(player->camera.position.y);
    int player_head_z = (int)roundf(player->camera.position.z);
    int player_feet_y = player_head_y - 1;

    if ((new_x == player_head_x && new_y == player_head_y &&
         new_z == player_head_z) ||
        (new_x == player_head_x && new_y == player_feet_y &&
         new_z == player_head_z)) {
      return; // Don't place block inside player
    }

    // Only place a block if the target space is empty
    if (GetVoxelID(voxel_data, new_x, new_y, new_z) == EMPTY) {
      PlaceVoxel(voxel_data, new_x, new_y, new_z, DIRT);
    }
  }
  EndPerformanceTracker("TryPlaceVoxel");
}

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

    int *mask = (int *)calloc(dims[0] * dims[1], sizeof(int));

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

          if (id1 != EMPTY && id2 == EMPTY) {
            mask[n++] = id1;
          } else if (id1 == EMPTY && id2 != EMPTY) {
            mask[n++] = -id2;
          } else {
            mask[n++] = 0;
          }
        }
      }

      x[axis]++;
      n = 0;

      for (int j = 0; j < dims[0]; ++j) {
        for (int i = 0; i < dims[1];) {
          if (mask[n] != 0) {
            int current_mask = mask[n];
            int w, h;
            for (w = 1; i + w < dims[1] && mask[n + w] == current_mask; ++w) {
            }

            bool done = false;
            for (h = 1; j + h < dims[0]; ++h) {
              for (int k = 0; k < w; ++k) {
                if (mask[n + k + h * dims[1]] != current_mask) {
                  done = true;
                  break;
                }
              }
              if (done)
                break;
            }

            bool positive_face = current_mask > 0;
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

            Vector3 v1 = {(float)x[0], (float)x[1], (float)x[2]};
            Vector3 v2 = {(float)(x[0] + du[0]), (float)(x[1] + du[1]),
                          (float)(x[2] + du[2])};
            Vector3 v3 = {(float)(x[0] + du[0] + dv[0]),
                          (float)(x[1] + du[1] + dv[1]),
                          (float)(x[2] + du[2] + dv[2])};
            Vector3 v4 = {(float)(x[0] + dv[0]), (float)(x[1] + dv[1]),
                          (float)(x[2] + dv[2])};

            int vert_idx = (face_count - 1) * 4;

            if (positive_face) {
              vertices[vert_idx + 0] = v1;
              vertices[vert_idx + 1] = v2;
              vertices[vert_idx + 2] = v3;
              vertices[vert_idx + 3] = v4;
            } else {
              vertices[vert_idx + 0] = v4;
              vertices[vert_idx + 1] = v3;
              vertices[vert_idx + 2] = v2;
              vertices[vert_idx + 3] = v1;
            }

            texcoords[vert_idx + 0] = (Vector2){0, 0};
            texcoords[vert_idx + 1] = (Vector2){(float)w, 0};
            texcoords[vert_idx + 2] = (Vector2){(float)w, (float)h};
            texcoords[vert_idx + 3] = (Vector2){0, (float)h};

            Vector3 norm = {(float)q[0], (float)q[1], (float)q[2]};
            if (!positive_face) {
              norm.x *= -1;
              norm.y *= -1;
              norm.z *= -1;
            }
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
