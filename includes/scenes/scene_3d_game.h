#ifndef SCENE_3D_GAME_H
#define SCENE_3D_GAME_H

#include "std_includes.h"

#define MAX_BLOCKS 10000

typedef struct Block {
  Vector3 position;
  bool active;
} Block;

int scene_3d_game(void);

#endif // SCENE_3D_GAME_H
