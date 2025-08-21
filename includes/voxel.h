#ifndef VOXEL_H
#define VOXEL_H

// #include "player.h"
#include "player.h"
#include "std_includes.h"
#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

#define VOXEL_SIZE 1.0f

// #define X_MAX 32
// #define Z_MAX 32
// #define Y_MAX 32
#define CHUNK_SIZE 63
#define X_MAX CHUNK_SIZE
#define Z_MAX CHUNK_SIZE
#define Y_MAX 8

#define X_NEIGHBOUR_OFFSET 1
#define Z_NEIGHBOUR_OFFSET X_MAX
#define Y_NEIGHBOUR_OFFSET X_MAX *Z_MAX

#define NUMBER_OF_VOXELS X_MAX *Y_MAX *Z_MAX

/* Each coordinate will be represented by 6bits.
 * That means the coordinates cant exeed 63,
 * */

/* MSB:
 * iiiiiiii ffffff yyyyyy zzzzzz xxxxxx
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

// --- MASKS ---
#define VOXEL_MASK_POS 0x3F
#define VOXEL_MASK_FACE 0x3F
#define VOXEL_MASK_ID 0xFF

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

Mesh GenMeshCustom(Vector3 offset);

// Return all indicies for all neighbour in range
Voxel *GetNeighbourVoxels(u16 search_scope, Voxel *voxel_data);

// Allocate xzy-coordinates to voxels
void InitVoxel(Voxel *voxel_data, bool is_random);

void UpdateVisibility(Voxel *voxel_data);

u8 Voxel_GetPosX(Voxel v);

u8 Voxel_GetPosY(Voxel v);

u8 Voxel_GetPosZ(Voxel v);

bool Voxel_IsActive(Voxel v);

u64 GetVoxelIndex(int x, int y, int z);

void PlaceVoxel(Voxel *voxel_data, int x, int y, int z, VoxelID id);

bool RemoveVoxel(Voxel *voxel_data, Player *player, u64 screen_width,
                 u64 screen_height, float player_range);

void TryPlaceVoxel(Voxel *voxel_data, Player *player, u64 screen_width,
                   u64 screen_height, float player_range);

VoxelID GetVoxelID(Voxel *voxel_data, int x, int y, int z);

Mesh CulledMeshing(Voxel *voxel_data);
#endif // VOXEL_H
