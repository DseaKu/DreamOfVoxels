#ifndef VOXEL_H
#define VOXEL_H

#include "scene_voxel_game.h"
#include "std_includes.h"
#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

#define N_VOXEL_X CHUNK_SIZE
#define N_VOXEL_Z CHUNK_SIZE
#define N_VOXEL_Y 4

#define X_NEIGHBOUR_OFFSET 1
#define Z_NEIGHBOUR_OFFSET N_VOXEL_X
#define Y_NEIGHBOUR_OFFSET N_VOXEL_X *N_VOXEL_Z

#define VOXELS_IN_TOTAL N_VOXEL_X *N_VOXEL_Y *N_VOXEL_Z

/* Each coordinate will be represented by 6bits.
 * That means the coordinates cant exeed 63,
 * */

/* MSB:
 * typedef u64 Voxel; Is defined in 'scene_3d_game.h' to avoid cycle include
 *
 *|        | iiiiiiii | ffffff | yyyyyy | zzzzzz | xxxxxx |
 *|--------|----------|--------|--------|--------|--------|
 *|    bits| 8 bits   | 6 bits | 6 bits | 6 bits | 6 bits |
 *
 * 6bit = posX
 * 6bit = posy
 * 6bit = posz
 * 6bit = face direction
 * 8bit = texture
 */

// --- SHIFTS ---
#define VOXEL_SHIFT_POS_X 0
#define VOXEL_SHIFT_POS_Z 6
#define VOXEL_SHIFT_POS_Y 12
#define VOXEL_SHIFT_FACE 18
#define VOXEL_SHIFT_ID 24
#define VOXEL_SHIFT_XZ_POS_OFFSET 32

// --- MASKS ---
#define VOXEL_MASK_POS 0x3F
#define VOXEL_MASK_FACE 0x3F
#define VOXEL_MASK_ID 0xFF
#define VOXEL_MASK_XZ_POS_OFFSET 0x3FF

typedef enum {
  FACE_DIR_POS_X = 1 << 0, // Right
  FACE_DIR_NEG_X = 1 << 1, // Left
  FACE_DIR_POS_Z = 1 << 2, // Front
  FACE_DIR_NEG_Z = 1 << 3, // Back
  FACE_DIR_POS_Y = 1 << 4, // Top
  FACE_DIR_NEG_Y = 1 << 5, // Bottom
} VoxelFaceDirection;

typedef enum {
  EMPTY = 0x0,
  DIRT = 0x1,
  GRASS = 0x2,
  STONE = 0x3,
  SAND = 0x4,
  WOOD = 0x5,

} VoxelID;

// Return all surrounding voxels of a position, regardless if the position
// inbetween chunks
// -> we have to return chunk + index and voxel + index

// Return all indicies for all neighbour in range
Voxel *GetNeighbourVoxels(u16 search_scope, Voxel *voxel_data);

// Allocate xzy-coordinates to voxels
Voxel *InitVoxelPointer(bool is_random);

void UpdateVisibility(Chunk *chunk_data);

u8 Voxel_GetPosX(Voxel v);

u8 Voxel_GetPosY(Voxel v);

u8 Voxel_GetPosZ(Voxel v);

bool Voxel_IsActive(Voxel v);

Voxel GetVoxel_XZY(Voxel *voxel_data, Vector3 position);
u64 GetVoxelIndex(int x, int y, int z);

void PlaceVoxel(Voxel *voxel_data, int x, int y, int z, VoxelID id);

bool RemoveVoxel(Voxel *voxel_data, Player *player, u64 screen_width,
                 u64 screen_height, float player_range);

VoxelID GetLocalVoxelIndex(Voxel *voxel_data, int x, int y, int z);
VoxelIndex GetGlobalVoxelIndex(Chunk *chunk_data, s64Vector3D tar_v);
Vector3 GetGlobalVoxelPosition(Chunk *chunk_data, s64Vector3D tar_v);

void CulledMeshing(Chunk *chunk_data);
#endif // VOXEL_H
