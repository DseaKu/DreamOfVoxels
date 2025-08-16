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

/* Each coordinate will be represented by 6bits.
 * That means the coordinates cant exeed 63,
 * thats why I choose 62 to keep it even,
 * get the max storage value
 * */
#define NUMBER_OF_VOXELS X_MAX *Y_MAX *Z_MAX

typedef u64 Voxel;

/* MSB:
 * tttttttt ffffff zzzzzz yyyyyy xxxxxx
 * 6bit = posX
 * 6bit = posy
 * 6bit = posz
 * 3bit = face direction
 * 8bit = texture
 *
 * X = Depth
 * Z = Width
 * Y = Height
 *
 * Face directions
 * +X = 0b000001
 * -X = 0b000010
 * +Z = 0b000100
 * -Z = 0b001000
 * +Y = 0b010000
 * -Y = 0b100000
 *
 * Special states
 * */
typedef enum {
  FACE_DIR_POS_X = 0b000001,
  FACE_DIR_NEG_X = 0b000010,
  FACE_DIR_NEG_Z = 0b001000, // 8
  FACE_DIR_POS_Y = 0b010000, // 16
  FACE_DIR_NEG_Y = 0b100000, // 32
} VoxelFaceDirection;
Mesh GenMeshCustom(Vector3 offset);

// Mesh GenVertexMesh(Vertex vertices_data);

/* Voxel faces */
void InitVoxel(Voxel *voxel_data);
#endif // VOXEL_H
