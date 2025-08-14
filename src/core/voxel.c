#include "core/voxel.h"
#include <raylib.h>

void InitVoxel(Voxel *voxels) {
  for (u64 index = 0; index < VOXEL_XYZ; index++) {
    float x = index % VOXEL_X;
    float z = (index / VOXEL_Z) % VOXEL_Z;
    float y = ((index / VOXEL_Y) / VOXEL_Y) % VOXEL_Y;
    voxels[index].position = (Vector3){x, y, z};
  }
}

void RemoveVoxel() {}

void UpdateAllVoxel() {}

void DrawVoxel(Voxel *voxels) {}
