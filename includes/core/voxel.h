#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

#define VOXEL_SIZE 1.0f

#define X_MAX 62
#define Y_MAX 62
#define Z_MAX 62
#define NUMBER_OF_VOXELS X_MAX *Y_MAX *Z_MAX

/* Each coordinate will be represented by 6bits.
 * That means the coordinates cant exeed 63,
 * thats why I choose 62 to keep it even,
 * get the max storage value
 * */

/* MSB:
 * tttttttt ffffff zzzzzz yyyyyy xxxxxx
 * 6bit = posX
 * 6bit = posy
 * 6bit = posz
 * 3bit = face direction
 * 8bit = texture
 */

// --- SHIFTS ---
#define VOXEL_SHIFT_POS_X 0
#define VOXEL_SHIFT_POS_Y 6
#define VOXEL_SHIFT_POS_Z 12
#define VOXEL_SHIFT_FACE 18
#define VOXEL_SHIFT_TEXTURE 24

// --- MASKS ---
#define VOXEL_MASK_POS 0x3F
#define VOXEL_MASK_FACE 0x3F
#define VOXEL_MASK_TEXTURE 0xFF

typedef u64 Voxel;

/*
 * X = Depth
 * Z = Width
 * Y = Height
 *
 * */
typedef enum {
  FACE_DIR_POS_X = 0b000001, // Rear
  FACE_DIR_NEG_X = 0b000010, // Front
  FACE_DIR_NEG_Z = 0b001000, // Left
  FACE_DIR_POS_Y = 0b010000, // Top
  FACE_DIR_NEG_Y = 0b100000, // Bottom
} VoxelFaceDirection;

Mesh GenMeshCustom(Vector3 offset);

// Mesh GenVertexMesh(Vertex vertices_data);

// Allocate xzy-coordinates to voxels
void InitVoxel(Voxel *voxel_data);

// Gets the X position from a voxel
u8 Voxel_GetPosX(Voxel v) {
  return (u8)((v >> VOXEL_SHIFT_POS_X) & VOXEL_MASK_POS);
}

// Gets the Y position from a voxel
u8 Voxel_GetPosY(Voxel v) {
  return (u8)((v >> VOXEL_SHIFT_POS_Y) & VOXEL_MASK_POS);
}

// Gets the Z position from a voxel
u8 Voxel_GetPosZ(Voxel v) {
  return (u8)((v >> VOXEL_SHIFT_POS_Z) & VOXEL_MASK_POS);
}

#endif // VOXEL_H
