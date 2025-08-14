#include "core/voxel.h"
#include <raylib.h>

void InitVoxel(Voxel *voxels) {
  for (u8 x = 0; x < VOXEL_X; x++) {

    for (u8 z = 0; z < VOXEL_Z; z++) {
      for (u8 y = 0; y < VOXEL_Y; y++) {
        u64 index = x * VOXEL_Z * VOXEL_Y + z * VOXEL_Y + y;
        voxels[index].position = (Vector3){x, y, z};
      }
    }
  }
}

void RemoveVoxel() {}

void UpdateAllVoxel() {}

void DrawVoxel(Voxel *voxels) {}
