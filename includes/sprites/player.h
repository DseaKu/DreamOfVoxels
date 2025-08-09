#ifndef PLAYER_H
#define PLAYER_H

#include "std_includes.h"

typedef struct Player {
  Camera camera;
  Vector3 position;
  float speed;
} Player;

Player InitPlayer(void);
void UpdatePlayer(Player *player);

#endif // PLAYER_H
