#include "core/voxel.h"
#include "std_includes.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

// void InitVoxel(Voxel *voxels) {
//   for (u8 x = 0; x < VOXEL_X; x++) {
//     for (u8 z = 0; z < VOXEL_Z; z++) {
//       for (u8 y = 0; y < VOXEL_Y; y++) {
//         u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
//         voxels[index].position = (Vector3){x, y, z};
//         voxels[index].is_visible = true;
//         voxels[index].id = SOLID;
//
//         // Disable visibilty of all z direction
//         // if (z != 0) {
//         //   voxels[index].is_visible = false;
//         // }
//       }
//     }
//   }
// }

void RemoveVoxel() {}

void UpdateVoxel(Voxel *voxels) {
  // No-op for now. Voxel visibility is determined at mesh generation time.
}

// void DrawVoxelSimple(Voxel *voxels) {
//   for (u8 x = 0; x < VOXEL_X; x++) {
//     for (u8 z = 0; z < VOXEL_Z; z++) {
//       for (u8 y = 0; y < VOXEL_Y; y++) {
//         u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
//
//         if (voxels[index].id == EMPTY || voxels[index].is_visible == false) {
//           continue;
//         }
//         DrawCubeWires(voxels[index].position, VOXEL_SIZE, VOXEL_SIZE,
//                       VOXEL_SIZE, MAROON);
//         DrawCube(voxels[index].position, VOXEL_SIZE, VOXEL_SIZE, VOXEL_SIZE,
//                  GRAY);
//       }
//     }
//   }
// }

// DrawModelSimple is no longer needed as we now draw a single, combined mesh.

Mesh BuildSingelVoxelMesh() {
  Mesh mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  return mesh;
}

// Mesh BuildVoxelFaceMesh(Voxel *voxels) {
//   int max_vertices = VOXEL_XYZ * 6 * 6; // Max possible vertices
//   float *vertices = (float *)MemAlloc(max_vertices * 3 * sizeof(float));
//   float *texcoords = (float *)MemAlloc(max_vertices * 2 * sizeof(float));
//   float *normals = (float *)MemAlloc(max_vertices * 3 * sizeof(float));
//   int vc = 0; // vertex count
//
//   float s2 = VOXEL_SIZE / 2.0f;
//
//   for (int x = 0; x < VOXEL_X; x++) {
//     for (int z = 0; z < VOXEL_Z; z++) {
//       for (int y = 0; y < VOXEL_Y; y++) {
//         u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
//         if (voxels[index].id == EMPTY)
//           continue;
//
//         Vector3 p = voxels[index].position;
//
// #define ADD_VERTEX(vx, vy, vz) \
//   vertices[vc * 3 + 0] = (vx); \
//   vertices[vc * 3 + 1] = (vy); \ vertices[vc * 3 + 2] = (vz)
// #define ADD_NORMAL(nx, ny, nz) \
//   normals[vc * 3 + 0] = (nx); \
//   normals[vc * 3 + 1] = (ny); \ normals[vc * 3 + 2] = (nz)
// #define ADD_TEXCOORDS(u, v) \
//   texcoords[vc * 2 + 0] = (u); \ texcoords[vc * 2 + 1] = (v)
// #define NEXT_VERTEX() vc++
//
//         // Check Back Face (-Z)
//         if (z == 0 || voxels[index - VOXEL_Y].id == EMPTY) {
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(0, 0, -1);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(0, 0, -1);
//           ADD_TEXCOORDS(0, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(0, 0, -1);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(0, 0, -1);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(0, 0, -1);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(0, 0, -1);
//           ADD_TEXCOORDS(1, 0);
//           NEXT_VERTEX();
//         }
//
//         // Check Front Face (+Z)
//         if (z == VOXEL_Z - 1 || voxels[index + VOXEL_Y].id == EMPTY) {
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(0, 0, 1);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(0, 0, 1);
//           ADD_TEXCOORDS(0, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(0, 0, 1);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(0, 0, 1);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(0, 0, 1);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(0, 0, 1);
//           ADD_TEXCOORDS(1, 0);
//           NEXT_VERTEX();
//         }
//
//         // Check Bottom Face (-Y)
//         if (y == 0 || voxels[index - 1].id == EMPTY) {
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(0, -1, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(0, -1, 0);
//           ADD_TEXCOORDS(0, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(0, -1, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(0, -1, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(0, -1, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(0, -1, 0);
//           ADD_TEXCOORDS(1, 0);
//           NEXT_VERTEX();
//         }
//
//         // Check Top Face (+Y)
//         if (y == VOXEL_Y - 1 || voxels[index + 1].id == EMPTY) {
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(0, 1, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(0, 1, 0);
//           ADD_TEXCOORDS(0, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(0, 1, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(0, 1, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(0, 1, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(0, 1, 0);
//           ADD_TEXCOORDS(1, 0);
//           NEXT_VERTEX();
//         }
//
//         // Check Left Face (-X)
//         if (x == 0 || voxels[index - VOXEL_Z * VOXEL_Y].id == EMPTY) {
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(-1, 0, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(-1, 0, 0);
//           ADD_TEXCOORDS(0, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(-1, 0, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(-1, 0, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(-1, 0, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x - s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(-1, 0, 0);
//           ADD_TEXCOORDS(1, 0);
//           NEXT_VERTEX();
//         }
//
//         // Check Right Face (+X)
//         if (x == VOXEL_X - 1 || voxels[index + VOXEL_Z * VOXEL_Y].id ==
//         EMPTY) {
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(1, 0, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z + s2);
//           ADD_NORMAL(1, 0, 0);
//           ADD_TEXCOORDS(0, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(1, 0, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z + s2);
//           ADD_NORMAL(1, 0, 0);
//           ADD_TEXCOORDS(0, 0);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y - s2, p.z - s2);
//           ADD_NORMAL(1, 0, 0);
//           ADD_TEXCOORDS(1, 1);
//           NEXT_VERTEX();
//           ADD_VERTEX(p.x + s2, p.y + s2, p.z - s2);
//           ADD_NORMAL(1, 0, 0);
//           ADD_TEXCOORDS(1, 0);
//           NEXT_VERTEX();
//         }
// #undef ADD_VERTEX
// #undef ADD_NORMAL
// #undef ADD_TEXCOORDS
// #undef NEXT_VERTEX
//       }
//     }
//   }

// Mesh mesh = {0};
// mesh.vertexCount = vc;
// mesh.triangleCount = vc / 3;
// mesh.vertices = (float *)MemRealloc(vertices, vc * 3 * sizeof(float));
// mesh.texcoords = (float *)MemRealloc(texcoords, vc * 2 * sizeof(float));
// mesh.normals = (float *)MemRealloc(normals, vc * 3 * sizeof(float));
//
// UploadMesh(&mesh, false);
// return mesh;
// }

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
/* MSB:
 * 0000 ttttttt fff zzzzzz yyyyyy xxxxxx
 * 6bits = posX
 * 6bits = posy
 * 6bits = posz
 * 3bits = face
 * 7bits = texture
 * 4bits = reserved
 * */

void InitVertex(Vertex *vertices_data) {
  u32 index = 0;
  // u32 a,yy,xx,zz = 0;

  for (u8 z = 0; z < Z_MAX; z++) {
    for (u8 y = 0; y < Y_MAX; y++) {
      for (u8 x = 0; x < X_MAX; x++) {

        // Shift the values into their correct positions and then OR them
        vertices_data[index] = (u32)x | ((u32)y << 6) | ((u32)z << 12);
        // a= (u32)x | ((u32)y << 6) | ((u32)z << 12);
        // xx = a & 63;
        // yy = (a >> 6)&63;
        // zz = (a >> 12)&63;
        index++;
      }
    }
  }
}

Mesh GenVerticesMesh(u32 *vertices_data) {
  // Initialize a mesh object
  int offset_y = 0;
  int offset_x = 0;
  int offset_z = 0;

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
  mesh.vertices[0] = 0 + offset_x;
  mesh.vertices[1] = 0 + offset_y;
  mesh.vertices[2] = 0 + offset_z;
  // Normal vector for Vertex 1 (pointing upwards)
  mesh.normals[0] = 0;
  mesh.normals[1] = 1;
  mesh.normals[2] = 0;
  // Texture coordinates for Vertex 1
  mesh.texcoords[0] = 0;
  mesh.texcoords[1] = 0;

  // Vertex 2: Position (1, 0, 2)
  mesh.vertices[3] = 1 + offset_x;
  mesh.vertices[4] = 0 + offset_y;
  mesh.vertices[5] = 2 + offset_z;
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
void InitVoxelFaces(Voxel *voxel_data) {
  u32 index = 0;

  // Shift the values into their correct positions and then OR them
  for (u8 z = 0; z < Z_MAX; z++) {
    for (u8 y = 0; y < Y_MAX; y++) {
      for (u8 x = 0; x < X_MAX; x++) {

        // for (u32 face = 0; face < NUMBER_OF_FACES; face++) {
        voxel_data[index] |= (u32)x | ((u32)y << 6) | ((u32)z << 12);
        // printf("%u\n", voxel_data[index]);
        index++;
        // }
      }
    }
  }
}
