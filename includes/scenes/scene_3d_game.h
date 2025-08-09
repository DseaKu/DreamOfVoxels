#include "../std_includes.h"
#define MAX_BLOCKS 10000

typedef struct Player {
  Camera camera;
  Vector3 position;
  float speed;
} Player;

typedef struct Block {
  Vector3 position;
  bool active;
} Block;

int scene_3d_game(void);
