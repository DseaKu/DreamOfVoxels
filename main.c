#include "includes/scenes/scene_3d_game.h"
#include <stdio.h>

int main(void) {

  if (!(scene_3d_game() == 0)) {
    printf("Error:\nscene_3d_game\n\n");
  }

  return 0;
}
