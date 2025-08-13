#ifndef SCENE_3D_GAME_H
#define SCENE_3D_GAME_H

#include "std_includes.h"

#define MAX_SCREEN_WIDTH 1200
#define MAX_SCREEN_HEIGHT 800

#define PLAYGROUND_Y_MAX 100
#define PLAYGROUND_X_MAX 100

#define TARGET_FPS 820
#define BLOCK_SIZE 1.0f
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2.0f)
#define MAX_PLACMENT_DISTANCE 1000.0

int Scene3DGame(void);

#endif // SCENE_3D_GAME_H
