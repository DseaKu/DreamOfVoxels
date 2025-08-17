#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

#define VOXEL_SIZE 1.0f

static const u8 X_MAX = 0b111111;
static const u8 Z_MAX = 0b111111;
static const u8 Y_MAX = 0b111111;
// #define X_MAX 62
// #define Y_MAX 62
// #define Z_MAX 62
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

typedef u32 Voxel;

typedef enum {
  FACE_DIR_POS_X = 0b000001, // Right
  FACE_DIR_NEG_X = 0b000010, // Left
  FACE_DIR_POS_Z = 0b000100, // Front
  FACE_DIR_NEG_Z = 0b001000, // Back
  FACE_DIR_POS_Y = 0b010000, // Top
  FACE_DIR_NEG_Y = 0b100000, // Bottom
} VoxelFaceDirection;

typedef enum {
  EMPTY = 0x0,
  DIRT = 0x1,
  WATER = 0x2,

} VoxelID;

Mesh GenMeshCustom(Vector3 offset);

// Mesh GenVertexMesh(Vertex vertices_data);

// Allocate xzy-coordinates to voxels
void InitVoxel(Voxel *voxel_data);

// Gets the X position from a voxel
// u8 Voxel_GetPosX(Voxel v);

// Gets the Y position from a voxel
// u8 Voxel_GetPosY(Voxel v);

// Gets the Z position from a voxel
// u8 Voxel_GetPosZ(Voxel v);

void UpdateVisibiltyVoxelFaces(Voxel *voxel_data);
void UpdateVisibilty(Voxel *voxel_data);

bool IsDirPosXNeighbour(Voxel v);
#endif // VOXEL_H
