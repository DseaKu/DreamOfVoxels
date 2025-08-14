#ifndef SCENE_3D_GAME_H
#define SCENE_3D_GAME_H

#define MAX_SCREEN_WIDTH 1400
#define MAX_SCREEN_HEIGHT 900
#define CURSOR_RADIUS 2

// Flutter array
// To get index back to 3D position
// x = index % VOXEL_X;
// z = (index/VOXEL_Z) % VOXEL_Z;
// y = ((index/VOXEL_Y)/VOXEL_Y)%VOXEL_Y;
#define VOXEL_X 64
#define VOXEL_Y 64
#define VOXEL_Z 64
#define VOXEL_XYZ (1 + (VOXEL_Z) + (VOXEL_Y * VOXEL_Y))

#define TARGET_FPS 4000
#define BLOCK_SIZE 1.0f
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2.0f)
#define MAX_PLACMENT_DISTANCE 1000.0

int Scene3DGame(void);

#endif // SCENE_3D_GAME_H
