#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <stdint.h>

// Flattening array
// To read coordinates
// x = index % VOXEL_X;
// z = (index/VOXEL_Z) % VOXEL_Z;
// y = ((index/VOXEL_Y)/VOXEL_Y)%VOXEL_Y;
#define VOXEL_X 10
#define VOXEL_Y 10
#define VOXEL_Z 10
#define VOXEL_XYZ VOXEL_X *VOXEL_Y *VOXEL_Z
#define VOXEL_SIZE 1.0f

typedef enum {
  air = 0,
  solid,
  dirt,
  water,
} VoxelID;

typedef struct Voxel {
  VoxelID voxel_id;
  Vector3 position;
} Voxel;

typedef struct Vertex {
  float x;
  float y;
  float z;

  u16 id;
} Vertex;

void InitVoxel(Voxel *voxels);

void RemoveVoxel();

void GenerateMesh(Voxel *voxels);

void UpdateAllVoxel();

Mesh GenMeshCustom(Vector3 offset);

void DrawCubesNaive(Voxel *voxels);

#endif // VOXEL_H
