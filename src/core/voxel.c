#include "core/voxel.h"
#include "std_includes.h"
#include "utils/rescource_tracker.h"
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

void UpdateVisibilty(Voxel *voxel_data) {
  StartPerformanceTracker("Update visibilty");

  for (u64 index = 0; index < NUMBER_OF_VOXELS; index++) {
    Voxel v = voxel_data[index];

    u8 visible_faces = 0;
    u32 index_offset = index + (1 << VOXEL_SHIFT_POS_X);
    u32 neigh_pos_x = voxel_data[index + (1 << VOXEL_SHIFT_POS_X)];
    u32 ID_neigh_pos_x =
        (voxel_data[index + (1 << VOXEL_SHIFT_POS_X)] >> VOXEL_SHIFT_ID);
    u8 ID_value =
        ((voxel_data[index + (1 << VOXEL_SHIFT_POS_X)] >> VOXEL_SHIFT_ID) &
         VOXEL_MASK_ID);

    // Check if positive x neighbour (back) is EMPTY
    if (((voxel_data[index + (1 << VOXEL_SHIFT_POS_X)] >> VOXEL_SHIFT_ID) &
         VOXEL_MASK_ID) == 0) {
      visible_faces |= FACE_DIR_POS_X;
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
