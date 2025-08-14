#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <stdint.h>

// Flutter array
// To get index back to 3D position
// x = index % VOXEL_X;
// z = (index/VOXEL_Z) % VOXEL_Z;
// y = ((index/VOXEL_Y)/VOXEL_Y)%VOXEL_Y;
#define VOXEL_X 8
#define VOXEL_Y 4
#define VOXEL_Z 6
#define VOXEL_XYZ (1 + (VOXEL_Z) + (VOXEL_Y * VOXEL_Y))

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

void DrawVoxel(Voxel *voxels);

void UpdateAllVoxel();

#endif // VOXEL_H
