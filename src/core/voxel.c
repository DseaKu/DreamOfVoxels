#include "core/voxel.h"
#include "std_includes.h"
#include <raylib.h>
#include <stdbool.h>

void InitVoxel(Voxel *voxels) {
  for (u8 x = 0; x < VOXEL_X; x++) {

    for (u8 z = 0; z < VOXEL_Z; z++) {
      for (u8 y = 0; y < VOXEL_Y; y++) {
        u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
        voxels[index].position = (Vector3){x, y, z};
        voxels[index].is_visible = true;
        voxels[index].id = SOLID;
      }
    }
  }
}

void RemoveVoxel() {}

void UpdateVoxel(Voxel *voxels) {}

void DrawVoxelSimple(Voxel *voxels) {
  for (u8 x = 0; x < VOXEL_X; x++) {

    for (u8 z = 0; z < VOXEL_Z; z++) {
      for (u8 y = 0; y < VOXEL_Y; y++) {
        u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
        if (voxels[index].id == EMPTY || voxels[index].is_visible == false) {
          continue;
        }

        // Fly my little bird!
        // voxels[index].position.x += 0.1f;
        // voxels[index].position.y += 0.1f;
        // voxels[index].position.z += 0.1f;
        DrawCube(voxels[index].position, VOXEL_SIZE, VOXEL_SIZE, VOXEL_SIZE,
                 GRAY);
        DrawCubeWires(voxels[index].position, VOXEL_SIZE, VOXEL_SIZE,
                      VOXEL_SIZE, MAROON);
      }
    }
  }
}

Mesh BuildSingleVoxelMesh() {
  Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  return mesh;
}

Mesh BuildVoxelFaceMesh(Voxel *voxels) {
  Mesh mesh;
  mesh = GenMeshPlane(VOXEL_SIZE, VOXEL_SIZE, 4, 3);
  // for (u8 x = 0; x < VOXEL_X; x++) {
  //   for (u8 z = 0; z < VOXEL_Z; z++) {
  //     for (u8 y = 0; y < VOXEL_Y; y++) {
  //       // u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
  //     }
  //   }
  // }
  return mesh;
};

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
