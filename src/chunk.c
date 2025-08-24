#include "chunk.h"
#include "voxel.h"

void FreeAllChunkData(Chunk *chunk_data) {
  for (u8 i = 0; i < CHUNKS_IN_TOTAL; i++) {
    UnloadMesh(chunk_data[i].chunk_mesh);
    free(chunk_data[i].p_voxel_data);
  }
}

Chunk *InitChunks() {
  u8 i = 0;

  Chunk *chunk_data = calloc(CHUNKS_IN_TOTAL, sizeof(Chunk));

  for (u8 x = 0; x < N_CHUNKS_X; x++) {
    for (u8 z = 0; z < N_CHUNKS_Z; z++) {
      chunk_data[i].p_voxel_data = InitVoxelPointer(true);
      chunk_data[i].position =
          (s16Vector2D){x - (N_CHUNKS_X / 2), z - (N_CHUNKS_Z / 2)};
      chunk_data[i].is_mesh_dirty = true;
      i++;
    }
  }
  return chunk_data;
}
