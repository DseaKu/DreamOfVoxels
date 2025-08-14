#include "core/voxel.h"
#include <raylib.h>

void InitVoxel(Voxel *voxels) {
  for (u64 index = 0; index < VOXEL_XYZ; index++) {
    u8 x = index % VOXEL_X;
    u8 z = (index / VOXEL_Z) % VOXEL_Z;
    u8 y = ((index / VOXEL_Y) / VOXEL_Y) % VOXEL_Y;
  }
}

void RemoveVoxel() {}

void UpdateAllVoxel() {}

void DrawVoxel(Voxel *voxels) {}
