#include "core/voxel.h"
#include "rlgl.h"
#include "std_includes.h"
#include "utils/resource_tracker.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

Mesh BuildSingelVoxelMesh() {
  Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  return mesh;
}

// Init xzy-coordinates to voxels
void InitVoxel(Voxel *voxel_data) {
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

        v |= ((Voxel)DIRT << VOXEL_SHIFT_ID);

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

void PlaceVoxel(Voxel *voxel_data, int x, int y, int z, VoxelID id) {
  u64 index = GetVoxelIndex(x, y, z);
  if (index == -1) {
    return;
  }

  // Clear the old ID and set the new one
  voxel_data[index] &= ~((Voxel)VOXEL_MASK_ID << VOXEL_SHIFT_ID);
  voxel_data[index] |= ((Voxel)id << VOXEL_SHIFT_ID);
}

void RemoveVoxel(Voxel *voxel_data, Player *player, u64 screen_width,
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
}

void RenderVoxelFaces(Voxel *voxel_data, Texture2D texture) {
  StartPerformanceTracker("RenderVoxelFaces");
  rlSetTexture(texture.id);

  rlBegin(RL_QUADS);

  for (u64 index = 0; index < NUMBER_OF_VOXELS; index++) {
    Voxel v = voxel_data[index];

    // Skip EMPTY voxels
    if (((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID) == EMPTY) {
      continue;
    }

    u8 visible_faces = (v >> VOXEL_SHIFT_FACE) & VOXEL_MASK_FACE;

    // If no faces are visible, skip.
    if (visible_faces == 0) {
      continue;
    }

    // Get voxel position
    float x = (float)Voxel_GetPosX(v);
    float y = (float)Voxel_GetPosY(v);
    float z = (float)Voxel_GetPosZ(v);

    // Determine color based on VoxelID
    VoxelID id = (VoxelID)((v >> VOXEL_SHIFT_ID) & VOXEL_MASK_ID);
    if (id == DIRT) {
      // Use white to not tint the texture
      rlColor4ub(255, 255, 255, 255);
    } else if (id == WATER) {
      // Use blue for water
      rlColor4ub(0, 0, 255, 128);
    }

    // Check and draw each visible face (counter-clockwise order)
    if (visible_faces & FACE_DIR_POS_X) { // Right face
      rlNormal3f(1.0f, 0.0f, 0.0f);
      rlTexCoord2f(1.0f, 1.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
      rlTexCoord2f(1.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
      rlTexCoord2f(0.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
      rlTexCoord2f(0.0f, 1.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_NEG_X) { // Left face
      rlNormal3f(-1.0f, 0.0f, 0.0f);
      rlTexCoord2f(0.0f, 1.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
      rlTexCoord2f(0.0f, 0.0f);
      rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
      rlTexCoord2f(1.0f, 0.0f);
      rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
      rlTexCoord2f(1.0f, 1.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
    }
    if (visible_faces & FACE_DIR_POS_Y) { // Top face
      rlNormal3f(0.0f, 1.0f, 0.0f);
      rlTexCoord2f(0.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
      rlTexCoord2f(1.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
      rlTexCoord2f(1.0f, 1.0f);
      rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
      rlTexCoord2f(0.0f, 1.0f);
      rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_NEG_Y) { // Bottom face
      rlNormal3f(0.0f, -1.0f, 0.0f);
      rlTexCoord2f(1.0f, 1.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
      rlTexCoord2f(0.0f, 1.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
      rlTexCoord2f(0.0f, 0.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
      rlTexCoord2f(1.0f, 0.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_POS_Z) { // Front face
      rlNormal3f(0.0f, 0.0f, 1.0f);
      rlTexCoord2f(1.0f, 1.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
      rlTexCoord2f(1.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
      rlTexCoord2f(0.0f, 0.0f);
      rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
      rlTexCoord2f(0.0f, 1.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    }
    if (visible_faces & FACE_DIR_NEG_Z) { // Back face
      rlNormal3f(0.0f, 0.0f, -1.0f);
      rlTexCoord2f(0.0f, 1.0f);
      rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
      rlTexCoord2f(0.0f, 0.0f);
      rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
      rlTexCoord2f(1.0f, 0.0f);
      rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
      rlTexCoord2f(1.0f, 1.0f);
      rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
    }
  }
  rlEnd();
  rlSetTexture(0);
  EndPerformanceTracker("RenderVoxelFaces");
}
