#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

#define VOXEL_SIZE 1.0f

/* Each coordinate will be represented by 6bits.
 * That means the coordinates cant exeed 63,
 * thats why I choose 60 to keep it even,
 * get the max value and have some room for special states*/
#define X_MAX 60
#define Y_MAX 60
#define Z_MAX 60
#define NUMBER_OF_VOXELS X_MAX *Y_MAX *Z_MAX

typedef u32 Voxel;
typedef u32 Vertex;
/* MSB:
 * 0000 tttt ffffff zzzzzz yyyyyy xxxxxx
 * 6bits = posX
 * 6bits = posy
 * 6bits = posz
 * 3bits = face direction
 * 4bits = texture
 * 4bits = reserved
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
 * 0x3FFFF voxel disabled
 * */

Mesh GenMeshCustom(Vector3 offset);

Mesh GenVertexMesh(Vertex vertices_data);

/* Voxel faces */
// void InitVoxel(Voxel *voxel_data);
#endif // VOXEL_H
