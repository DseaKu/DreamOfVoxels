#include "voxel.h"
#include "resource_tracker.h"
#include "rlgl.h"
#include "scene_3d_game.h"
#include "std_includes.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
// Init xzy-coordinates to voxels
Voxel *InitVoxelPointer(bool is_random) {
  // 0.527000 ms (avg over 1 runs):Init Voxels
  Voxel *voxel_data = (Voxel *)calloc(VOXELS_IN_TOTAL, sizeof(Voxel));
  StartPerformanceTracker("Init Voxels");
  u64 index = 0;

  for (u8 y = 0; y < N_VOXEL_Y; y++) {
    for (u8 z = 0; z < N_VOXEL_Z; z++) {
      for (u8 x = 0; x < N_VOXEL_X; x++) {

        Voxel v = 0;

        // Pack the data using the named shift constants
        v |= ((Voxel)x << VOXEL_SHIFT_POS_X);
        v |= ((Voxel)y << VOXEL_SHIFT_POS_Y);
        v |= ((Voxel)z << VOXEL_SHIFT_POS_Z);

        if (!is_random) {
          v |= ((Voxel)WOOD << VOXEL_SHIFT_ID);
        } else {
          VoxelID random_id =
              (rand() % sizeof(VoxelID)) + 1; // DIRT, GRASS, STONE, SAND
          v |= ((Voxel)random_id << VOXEL_SHIFT_ID);
        }

        voxel_data[index] = v;
        index++;
      }
    }
  }
  EndPerformanceTracker("Init Voxels");
  return voxel_data;
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

bool Voxel_IsActive(Voxel v) { return ((v >> 24) & VOXEL_MASK_ID) != 0; }

void UpdateVisibility(Chunk *chunk_data) {
  StartPerformanceTracker("Update visibilty");
  for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
    Voxel *voxel_data = chunk_data[i].p_voxel_data;

    u64 index = 0;
    for (u8 y = 0; y < N_VOXEL_Y; y++) {
      for (u8 z = 0; z < N_VOXEL_Z; z++) {
        for (u8 x = 0; x < N_VOXEL_X; x++) {

          Voxel v = voxel_data[index];
          u8 visible_faces = 0;

          // Skip EMPTY voxel
          if (((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID) == EMPTY) {
            index++;
            continue;
          }

          // For voxels not on the boundary, skip boundary checks.
          if (x > 0 && x < N_VOXEL_X - 1 && y > 0 && y < N_VOXEL_Y - 1 &&
              z > 0 && z < N_VOXEL_Z - 1) {
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
            if (x == N_VOXEL_X - 1 ||
                ((voxel_data[index + X_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
                 VOXEL_MASK_ID) == EMPTY) {
              visible_faces |= FACE_DIR_POS_X;
            }
            if (x == 0 ||
                ((voxel_data[index - X_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
                 VOXEL_MASK_ID) == EMPTY) {
              visible_faces |= FACE_DIR_NEG_X;
            }
            if (z == N_VOXEL_Z - 1 ||
                ((voxel_data[index + Z_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
                 VOXEL_MASK_ID) == EMPTY) {
              visible_faces |= FACE_DIR_POS_Z;
            }
            if (z == 0 ||
                ((voxel_data[index - Z_NEIGHBOUR_OFFSET] >> VOXEL_SHIFT_ID) &
                 VOXEL_MASK_ID) == EMPTY) {
              visible_faces |= FACE_DIR_NEG_Z;
            }
            if (y == N_VOXEL_Y - 1 ||
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
  }
  EndPerformanceTracker("Update visibilty");
}

u64 GetVoxelIndex(int x, int y, int z) {
  if (x < 0 || x >= N_VOXEL_X || y < 0 || y >= N_VOXEL_Y || z < 0 ||
      z >= N_VOXEL_Z) {
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
  for (u64 index = 0; index < VOXELS_IN_TOTAL; index++) {
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
  for (u64 index = 0; index < VOXELS_IN_TOTAL; index++) {
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

void CulledMeshing(Chunk *chunk_data) {
  StartPerformanceTracker("GenerateGreedyMesh");
  for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
    if (chunk_data[i].is_dirty == false) {
      continue;
    }
    Voxel *voxel_data = chunk_data[i].p_voxel_data;
    // Generates a mesh from all the voxels in the world.
    // It uses a "naive" or "culled" meshing approach, where it creates a
    // separate quad (two triangles) for each face of a voxel that is visible
    // (i.e., not touching another voxel). This is simpler than a true "greedy"
    // mesh algorithm but can result in more vertices.

    // --- 1. Memory Allocation ---
    // Pre-allocate memory for the maximum possible number of vertices, texture
    // coordinates, and indices. This avoids reallocating memory in the loop,
    // which would be slow. Max vertices = total voxels * 6 faces/voxel * 4
    // vertices/face. Max indices = total voxels * 6 faces/voxel * 6
    // indices/face (2 triangles/face).
    int max_vertices = VOXELS_IN_TOTAL * 6 * 4;
    int max_indices = VOXELS_IN_TOTAL * 6 * 6;
    float *vertices = (float *)malloc(max_vertices * 3 * sizeof(float));
    float *texcoords = (float *)malloc(max_vertices * 2 * sizeof(float));
    unsigned short *indices =
        (unsigned short *)malloc(max_indices * sizeof(unsigned short));

    // Counters for the actual number of vertices and indices we add.
    int vertex_count = 0;
    int index_count = 0;

    // --- 2. Voxel Iteration ---
    // Loop through every potential voxel position in the world data.
    for (u64 i = 0; i < VOXELS_IN_TOTAL; i++) {
      Voxel v = voxel_data[i];

      // Extract voxel properties using bitwise operations.
      VoxelID id = (VoxelID)((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID);
      u8 visible_faces = (v >> VOXEL_SHIFT_FACE) & VOXEL_MASK_FACE;

      // Skip this voxel if it's empty (air) or has no visible faces.
      if (id == EMPTY || visible_faces == 0) {
        continue;
      }

      // Get the local coordinates (x, y, z) of the voxel.
      float x = (float)Voxel_GetPosX(v) * VOXEL_SIZE;
      float y = (float)Voxel_GetPosY(v) * VOXEL_SIZE;
      float z = (float)Voxel_GetPosZ(v) * VOXEL_SIZE;

      // --- 3. Texture Coordinate Calculation ---
      // Define the layout of the 2D texture atlas (we assume a 2x2 grid).
      const int atlas_cols = 16;
      const int atlas_rows = 16;
      // Calculate the width and height of a single texture within the atlas (in
      // UV space, 0.0 to 1.0).
      float tex_width = 1.0f / atlas_cols;
      float tex_height = 1.0f / atlas_rows;
      float u_base = 0.0f; // Base U coordinate (horizontal)
      float v_base = 0.0f; // Base V coordinate (vertical)

      // Determine the base U and V coordinates in the atlas based on the
      // voxel's ID. EMPTY = 0x0, DIRT = 0x1, GRASS = 0x2, STONE = 0x3, SAND =
      // 0x4,
      switch (id) {
      case GRASS:
        u_base = 3.0f * tex_width;
        v_base = 0.0f * tex_height;
        break; // Top-left
      case WOOD:
        u_base = 5.0f * tex_width;
        v_base = 0.0f * tex_height;
        break; // Top-right
      case STONE:
        u_base = 0.0f * tex_width;
        v_base = 1.0f * tex_height;
        break; // Top-right
      case DIRT:
        u_base = 2.0f * tex_width;
        v_base = 0.0f * tex_height;
        break; // Bottom-left
      case SAND:
        u_base = 2.0f * tex_width;
        v_base = 1.0f * tex_height;
        break; // Bottom-right
      default:
        break;
      }

      // --- 4. Vertex and UV Definition ---
      // Define the local-space vertex positions for a standard 1x1x1 cube.
      // Ordered by face.
      float half_size = VOXEL_SIZE / 2.0f;
      float v_data[] = {
          // Front (+Z)
          -half_size, -half_size,  half_size,
           half_size, -half_size,  half_size,
           half_size,  half_size,  half_size,
          -half_size,  half_size,  half_size,
          // Back (-Z)
           half_size, -half_size, -half_size,
          -half_size, -half_size, -half_size,
          -half_size,  half_size, -half_size,
           half_size,  half_size, -half_size,
          // Top (+Y)
          -half_size,  half_size,  half_size,
           half_size,  half_size,  half_size,
           half_size,  half_size, -half_size,
          -half_size,  half_size, -half_size,
          // Bottom (-Y)
          -half_size, -half_size, -half_size,
           half_size, -half_size, -half_size,
           half_size, -half_size,  half_size,
          -half_size, -half_size,  half_size,
          // Right (+X)
           half_size, -half_size,  half_size,
           half_size, -half_size, -half_size,
           half_size,  half_size, -half_size,
           half_size,  half_size,  half_size,
          // Left (-X)
          -half_size, -half_size, -half_size,
          -half_size, -half_size,  half_size,
          -half_size,  half_size,  half_size,
          -half_size,  half_size, -half_size
      };

      // Define the UV coordinates for a single quad, mapping to the correct
      // sub-texture in the atlas. (0,0) is top-left of the texture.
      float t_data[] = {
          u_base,
          v_base + tex_height, // Bottom-left vertex
          u_base + tex_width,
          v_base + tex_height, // Bottom-right vertex
          u_base + tex_width,
          v_base, // Top-right vertex
          u_base,
          v_base // Top-left vertex
      };

      // --- 5. Face Culling and Mesh Construction ---
      // Check each of the 6 directions. If a face is visible, add its geometry
      // to our arrays.
      if (visible_faces & FACE_DIR_POS_Z) { // Front face
        int v_offset = 0; // Vertices for this face start at index 0 in v_data
        for (int j = 0; j < 4; j++) { // Add 4 vertices
          vertices[(vertex_count + j) * 3 + 0] =
              v_data[(v_offset + j) * 3 + 0] + x;
          vertices[(vertex_count + j) * 3 + 1] =
              v_data[(v_offset + j) * 3 + 1] + y;
          vertices[(vertex_count + j) * 3 + 2] =
              v_data[(v_offset + j) * 3 + 2] + z;
          texcoords[(vertex_count + j) * 2 + 0] = t_data[j * 2 + 0];
          texcoords[(vertex_count + j) * 2 + 1] = t_data[j * 2 + 1];
        }
        // Add 6 indices to form two triangles for the quad.
        indices[index_count++] = vertex_count;
        indices[index_count++] = vertex_count + 1;
        indices[index_count++] = vertex_count + 2;
        indices[index_count++] = vertex_count;
        indices[index_count++] = vertex_count + 2;
        indices[index_count++] = vertex_count + 3;
        vertex_count += 4; // Increment master vertex count
      }
      if (visible_faces & FACE_DIR_NEG_Z) { // Back face
        int v_offset = 4; // Vertices for this face start at index 4 in v_data
        for (int j = 0; j < 4; j++) {
          vertices[(vertex_count + j) * 3 + 0] =
              v_data[(v_offset + j) * 3 + 0] + x;
          vertices[(vertex_count + j) * 3 + 1] =
              v_data[(v_offset + j) * 3 + 1] + y;
          vertices[(vertex_count + j) * 3 + 2] =
              v_data[(v_offset + j) * 3 + 2] + z;
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
      if (visible_faces & FACE_DIR_POS_Y) { // Top face
        int v_offset = 8;
        for (int j = 0; j < 4; j++) {
          vertices[(vertex_count + j) * 3 + 0] =
              v_data[(v_offset + j) * 3 + 0] + x;
          vertices[(vertex_count + j) * 3 + 1] =
              v_data[(v_offset + j) * 3 + 1] + y;
          vertices[(vertex_count + j) * 3 + 2] =
              v_data[(v_offset + j) * 3 + 2] + z;
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
      if (visible_faces & FACE_DIR_NEG_Y) { // Bottom face
        int v_offset = 12;
        for (int j = 0; j < 4; j++) {
          vertices[(vertex_count + j) * 3 + 0] =
              v_data[(v_offset + j) * 3 + 0] + x;
          vertices[(vertex_count + j) * 3 + 1] =
              v_data[(v_offset + j) * 3 + 1] + y;
          vertices[(vertex_count + j) * 3 + 2] =
              v_data[(v_offset + j) * 3 + 2] + z;
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
      if (visible_faces & FACE_DIR_POS_X) { // Right face
        int v_offset = 16;
        for (int j = 0; j < 4; j++) {
          vertices[(vertex_count + j) * 3 + 0] =
              v_data[(v_offset + j) * 3 + 0] + x;
          vertices[(vertex_count + j) * 3 + 1] =
              v_data[(v_offset + j) * 3 + 1] + y;
          vertices[(vertex_count + j) * 3 + 2] =
              v_data[(v_offset + j) * 3 + 2] + z;
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
      if (visible_faces & FACE_DIR_NEG_X) { // Left face
        int v_offset = 20;
        for (int j = 0; j < 4; j++) {
          vertices[(vertex_count + j) * 3 + 0] =
              v_data[(v_offset + j) * 3 + 0] + x;
          vertices[(vertex_count + j) * 3 + 1] =
              v_data[(v_offset + j) * 3 + 1] + y;
          vertices[(vertex_count + j) * 3 + 2] =
              v_data[(v_offset + j) * 3 + 2] + z;
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
    }

    // --- 6. Finalize Mesh ---
    Mesh mesh = {0};
    mesh.vertexCount = vertex_count;
    mesh.triangleCount = index_count / 3;

    // Trim the allocated memory to the actual size needed.
    mesh.vertices =
        (float *)realloc(vertices, vertex_count * 3 * sizeof(float));
    mesh.texcoords =
        (float *)realloc(texcoords, vertex_count * 2 * sizeof(float));
    mesh.indices = (unsigned short *)realloc(
        indices, index_count * sizeof(unsigned short));

    // Upload the mesh data (vertices, texcoords, indices) to the GPU for
    // rendering.
    UploadMesh(&mesh,
               false); // We are providing our own CPU-side copy of the data.
    chunk_data[i].chunk_mesh = mesh;
  }
  EndPerformanceTracker("GenerateGreedyMesh");
}
