#include "scenes/scene_3d_game.h"
#include <stdio.h>

int main(void) {

  if (!(Scene3DGame() == 0)) {
    printf("Error:\nscene_3d_game\n\n");
  }

  return 0;
}
