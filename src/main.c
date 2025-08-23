#include "scene_voxel_game.h"
#include <stdio.h>

int main(void) {

  if (!(Scene3DGame() == 0)) {
    printf("Error at:\nScene3DGame\n\n");
  }

  return 0;
}
