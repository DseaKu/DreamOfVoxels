#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <stdint.h>

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

typedef struct SearchScope {
  const u8 x1;
  const u8 x2;
  const u8 y1;
  const u8 y2;
  const u8 z1;
  const u8 z2;
} SearchScope;

void InitVoxels();

void RemoveVoxel();

void DrawVoxel(Voxel *voxels);

void UpdateAllVoxel();

#endif // VOXEL_H
