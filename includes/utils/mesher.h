#ifndef MESHER_H
#define MESHER_H

#include "sprites/block.h"
#include <raylib.h>

Mesh GenerateGreedyMesh(
    Block blocks[PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX][PLAYGROUND_Z_MAX]);

#endif // MESHER_H
