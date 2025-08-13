#include "sprites/block.h"
#include <raylib.h>
#include <stdbool.h>

void InitBlocks(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {

  for (int z = 0; z < PLAYGROUND_Z_MAX; z++) {
    for (int x = 0; x < PLAYGROUND_X_MAX; x++) {
      for (int y = 0; y < PLAYGROUND_Y_MAX; y++) {
        blocks[z][x][y].state = FACE_ALL;
      }
    }
  }
}

void DrawCubeFace(
    Block blocks[PLAYGROUND_Z_MAX][PLAYGROUND_X_MAX][PLAYGROUND_Y_MAX]) {

  for (int z = 0; z < PLAYGROUND_Z_MAX; z++) {
    for (int x = 0; x < PLAYGROUND_X_MAX; x++) {
      for (int y = 0; y < PLAYGROUND_Y_MAX; y++) {
        if (blocks[z][x][y].state == 0) {
          continue; // Skip inactive blocks
        }

        // Get the block's state
        uint8_t state = blocks[z][x][y].state;
        Vector3 pos = {(float)x, (float)y, (float)z};
        Color color = LIGHTGRAY; // Face color
        Color wireColor = GRAY;  // Wireframe color

        // Define the 8 vertices of the cube, centered at the position
        Vector3 v1 = {pos.x - 0.5f, pos.y - 0.5f,
                      pos.z - 0.5f}; // Bottom-back-left
        Vector3 v2 = {pos.x + 0.5f, pos.y - 0.5f,
                      pos.z - 0.5f}; // Bottom-back-right
        Vector3 v3 = {pos.x + 0.5f, pos.y - 0.5f,
                      pos.z + 0.5f}; // Bottom-front-right
        Vector3 v4 = {pos.x - 0.5f, pos.y - 0.5f,
                      pos.z + 0.5f}; // Bottom-front-left
        Vector3 v5 =
            {pos.x - 0.5f, pos.y + 0.5f, pos.z - 0.5f}; // Top-back-left
        Vector3 v6 =
            {pos.x + 0.5f, pos.y + 0.5f, pos.z - 0.5f}; // Top-back-right
        Vector3 v7 =
            {pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f}; // Top-front-right
        Vector3 v8 =
            {pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.5f}; // Top-front-left

        // Draw visible faces
        if (state & FACE_TOP) {
          DrawTriangle3D(v5, v6, v8, color);
          DrawTriangle3D(v6, v7, v8, color);
          DrawLine3D(v5, v6, wireColor);
          DrawLine3D(v6, v7, wireColor);
          DrawLine3D(v7, v8, wireColor);
          DrawLine3D(v8, v5, wireColor);
        }
        if (state & FACE_BOTTOM) {
          DrawTriangle3D(v4, v3, v1, color);
          DrawTriangle3D(v3, v2, v1, color);
          DrawLine3D(v1, v2, wireColor);
          DrawLine3D(v2, v3, wireColor);
          DrawLine3D(v3, v4, wireColor);
          DrawLine3D(v4, v1, wireColor);
        }
        if (state & FACE_FRONT) {
          DrawTriangle3D(v8, v7, v4, color);
          DrawTriangle3D(v7, v3, v4, color);
          DrawLine3D(v8, v7, wireColor);
          DrawLine3D(v7, v3, wireColor);
          DrawLine3D(v3, v4, wireColor);
          DrawLine3D(v4, v8, wireColor);
        }
        if (state & FACE_BACK) {
          DrawTriangle3D(v5, v1, v6, color);
          DrawTriangle3D(v1, v2, v6, color);
          DrawLine3D(v5, v1, wireColor);
          DrawLine3D(v1, v2, wireColor);
          DrawLine3D(v2, v6, wireColor);
          DrawLine3D(v6, v5, wireColor);
        }
        if (state & FACE_LEFT) {
          DrawTriangle3D(v8, v4, v5, color);
          DrawTriangle3D(v4, v1, v5, color);
          DrawLine3D(v8, v4, wireColor);
          DrawLine3D(v4, v1, wireColor);
          DrawLine3D(v1, v5, wireColor);
          DrawLine3D(v5, v8, wireColor);
        }
        if (state & FACE_RIGHT) {
          DrawTriangle3D(v7, v6, v3, color);
          DrawTriangle3D(v6, v2, v3, color);
          DrawLine3D(v7, v6, wireColor);
          DrawLine3D(v6, v2, wireColor);
          DrawLine3D(v2, v3, wireColor);
          DrawLine3D(v3, v7, wireColor);
        }
      }
    }
  }
}

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
