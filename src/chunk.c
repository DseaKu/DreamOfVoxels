#include "chunk.h"
#include "scene_voxel_game.h"
#include "std_includes.h"
#include "voxel.h"
#include <math.h>
#include <stdbool.h>

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
      if (i > N_EMPTY_VOXEL) {
        chunk_data[i].p_voxel_data = InitVoxel(true, true);
      } else {
        chunk_data[i].p_voxel_data = InitVoxel(true, false);
      }
      chunk_data[i].position =
          (s16Vector2D){x - (N_CHUNKS_X / 2), z - (N_CHUNKS_Z / 2)};
      chunk_data[i].is_mesh_dirty = true;
      chunk_data[i].voxel_offset_x = chunk_data->position.x * N_VOXEL_X;
      chunk_data[i].voxel_offset_z = chunk_data->position.z * N_VOXEL_Z;
      i++;
    }
  }
  return chunk_data;
}
u32 GetIndexCurrentChunk(Player *player) {
  float chunk_size_x = N_VOXEL_X * VOXEL_SIZE;
  float chunk_size_z = N_VOXEL_Z * VOXEL_SIZE;

  int chunk_x_offset = floorf(player->body.position.x / chunk_size_x);
  int chunk_z_offset = floorf(player->body.position.z / chunk_size_z);

  int chunk_x_index = chunk_x_offset + (N_CHUNKS_X / 2);
  int chunk_z_index = chunk_z_offset + (N_CHUNKS_Z / 2);

  // Clamp chunk indices to be within the world boundaries
  if (chunk_x_index < 0)
    chunk_x_index = 0;
  if (chunk_x_index >= N_CHUNKS_X)
    chunk_x_index = N_CHUNKS_X - 1;
  if (chunk_z_index < 0)
    chunk_z_index = 0;
  if (chunk_z_index >= N_CHUNKS_Z)
    chunk_z_index = N_CHUNKS_Z - 1;

  return chunk_x_index * N_CHUNKS_Z + chunk_z_index;
}
s16Vector2D GetXZCurrentChunk(Player *player) {
  float chunk_size_x = N_VOXEL_X * VOXEL_SIZE;
  float chunk_size_z = N_VOXEL_Z * VOXEL_SIZE;

  return (s16Vector2D){floorf(player->body.position.x / chunk_size_x),
                       floorf(player->body.position.z / chunk_size_z)};
}
u64 GetIndexXZYChunk(Chunk *chunk_data, s64Vector3D xzy_coor) {
  float chunk_size_x = N_VOXEL_X * VOXEL_SIZE;
  float chunk_size_z = N_VOXEL_Z * VOXEL_SIZE;

  int chunk_x_offset = floorf(xzy_coor.x / chunk_size_x);
  int chunk_z_offset = floorf(xzy_coor.z / chunk_size_z);

  int chunk_x_index = chunk_x_offset + (N_CHUNKS_X / 2);
  int chunk_z_index = chunk_z_offset + (N_CHUNKS_Z / 2);

  // Clamp chunk indices to be within the world boundaries
  if (chunk_x_index < 0)
    chunk_x_index = 0;
  if (chunk_x_index >= N_CHUNKS_X)
    chunk_x_index = N_CHUNKS_X - 1;
  if (chunk_z_index < 0)
    chunk_z_index = 0;
  if (chunk_z_index >= N_CHUNKS_Z)
    chunk_z_index = N_CHUNKS_Z - 1;

  return chunk_x_index * N_CHUNKS_Z + chunk_z_index;
}
