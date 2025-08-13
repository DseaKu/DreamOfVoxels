#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void InitBlocks(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {}

void DrawCubeFace(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {}

void RemoveBlock(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {}

void UpdateAllBlockFaces(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {
  for (int z = 0; z < PLAYGROUND_Z_MAX; z++) {
    for (int x = 0; x < PLAYGROUND_X_MAX; x++) {
      for (int y = 0; y < PLAYGROUND_Y_MAX; y++) {
        if (blocks[z][x][y].state == 0) {
          continue; // Skip inactive blocks
        }

        // Start with all faces visible
        blocks[z][x][y].state = FACE_TOP | FACE_BOTTOM | FACE_FRONT |
                                FACE_BACK | FACE_LEFT | FACE_RIGHT;

        // Check top face
        if (y < PLAYGROUND_Y_MAX - 1 && blocks[z][x][y + 1].state != 0) {
          blocks[z][x][y].state &= ~FACE_TOP;
        }
        // Check bottom face
        if (y > 0 && blocks[z][x][y - 1].state != 0) {
          blocks[z][x][y].state &= ~FACE_BOTTOM;
        }
        // Check front face
        if (z < PLAYGROUND_Z_MAX - 1 && blocks[z + 1][x][y].state != 0) {
          blocks[z][x][y].state &= ~FACE_FRONT;
        }
        // Check back face
        if (z > 0 && blocks[z - 1][x][y].state != 0) {
          blocks[z][x][y].state &= ~FACE_BACK;
        }
        // Check right face
        if (x < PLAYGROUND_X_MAX - 1 && blocks[z][x + 1][y].state != 0) {
          blocks[z][x][y].state &= ~FACE_RIGHT;
        }
        // Check left face
        if (x > 0 && blocks[z][x - 1][y].state != 0) {
          blocks[z][x][y].state &= ~FACE_LEFT;
        }
      }
    }
  }
}
