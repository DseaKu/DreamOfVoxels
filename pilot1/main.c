#include "scenes/scene_3d_game.h"
#include <stdio.h>

int main(void) {

  if (!(Scene3DGame() == 0)) {
    printf("Error at:\ncene3DGame\n\n");
  }

  return 0;
}
