#ifndef VOXEL_H
#define VOXEL_H

#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

// Flattening array
// To read coordinates
// x = index % VOXEL_X;
// z = (index/VOXEL_Z) % VOXEL_Z;
// y = ((index/VOXEL_Y)/VOXEL_Y)%VOXEL_Y;
#define VOXEL_X 20
#define VOXEL_Y 20
#define VOXEL_Z 20
#define VOXEL_XYZ VOXEL_X *VOXEL_Y *VOXEL_Z
#define VOXEL_SIZE 1.0f

typedef enum {
  EMPTY = 0,
  SOLID,
  DIRT,
  WATER,
} ID;

typedef struct Voxel {
  ID id;
  Vector3 position;
  bool is_visible;
  // Mesh mesh;
} Voxel;

void InitVoxel(Voxel *voxels);

void RemoveVoxel();

void GenerateMesh(Voxel *voxels);

void UpdateVoxel(Voxel *voxels);

Mesh GenMeshCustom(Vector3 offset);

Mesh BuildSingelVoxelMesh();
Mesh BuildVoxelFaceMesh(Voxel *voxels);

void DrawVoxelSimple(Voxel *voxels);


#endif // VOXEL_H
