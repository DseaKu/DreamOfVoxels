#ifndef VOXEL_H
#define VOXEL_H

#include "std_includes.h"
#include <iso646.h>
#include <raylib.h>
#include <stdint.h>

// Flattening array
// To read coordinates
// x = index % VOXEL_X;
// z = (index/VOXEL_Z) % VOXEL_Z;
// y = ((index/VOXEL_Y)/VOXEL_Y)%VOXEL_Y;
//
#define VOXEL_X 64
#define VOXEL_Y 64
#define VOXEL_Z 64
#define VOXEL_XYZ VOXEL_X *VOXEL_Y *VOXEL_Z
#define VOXEL_SIZE 1.0f

typedef enum {
  EMPTY = 0,
  SOLID,
  DIRT,
  WATER,
} ID;

typedef struct Voxel {
  ID id;
  Vector3 position;
  bool is_visible;
  Mesh mesh;
} Voxel;

/* Each coordinate will be represented by 6bits.
 * That means the coordinates cant exeed 63,
 * thats why I choose 62 to keep it even
 * and get the max value */
#define X_MAX 3
#define Y_MAX 3
#define Z_MAX 3
#define NUMBER_OF_VOXELS X_MAX *Y_MAX *Z_MAX
#define NUMBER_OF_FACES NUMBER_OF_VOXELS * 6
#define NUMBER_OF_VERTICES NUMBER_OF_FACES * 2

typedef u32 Vertex;
/* MSB:
 * 0000 ttttttt fff zzzzzz yyyyyy xxxxxx
 * 6bits = posX
 * 6bits = posy
 * 6bits = posz
 * 3bits = face
 * 7bits = texture
 * 4bits = reserved
 * */

/* Voxel Section */
Voxel GetVoxel(u32 xyz);
void InitVoxel(Voxel *voxels);

void RemoveVoxel();

void GenerateMesh(Voxel *voxels);

void UpdateVoxel(Voxel *voxels);

Mesh GenMeshCustom(Vector3 offset);

Mesh BuildSingelVoxelMesh();
Mesh BuildVoxelFaceMesh(Voxel *voxels);

void DrawVoxelSimple(Voxel *voxels);

/* Vertex Section */

Mesh GenVertexMesh(Vertex vertices_data);
void InitVertex(Vertex *vertices_data);

#endif // VOXEL_H
