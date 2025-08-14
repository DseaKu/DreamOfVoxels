#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <stdint.h>

typedef enum {
  air = 0,
  solid,
  dirt,
  water,
} VoxelId;

typedef struct Voxel {
  VoxelId voxel_id;
  Vector3 position;
} Voxel;

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

void UpdateAllVoxel();

#endif // VOXEL_H
