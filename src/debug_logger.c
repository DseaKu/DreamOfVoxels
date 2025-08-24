#include "debug_logger.h"
#include "chunk.h"
#include "resource_tracker.h"
#include "voxel.h"
#include <rlgl.h>
#include <stdarg.h>
#include <stdio.h>

char debug_message[256] = {0};
static double debug_message_set_time = 0.0;

void SetDebugMessage(const char *message, ...) {
  va_list args;
  va_start(args, message);
  vsnprintf(debug_message, sizeof(debug_message), message, args);
  va_end(args);
  debug_message_set_time = GetTime();
}

void DrawDebugMessages(void) {
  if (GetTime() - debug_message_set_time < 0.001) {
    DrawText("Debug text:", 30, 20, 10, MAROON);
    DrawText(debug_message, 30, 40, 10, MAROON);
  }
}
void Draw3DDebugInformation(int screen_width, int screen_height) {

  StartPerformanceTracker("Draw 3D debug information");
  rlPushMatrix();
  rlTranslatef(VOXEL_SIZE / 2.0f, -VOXEL_SIZE / 2.0f, VOXEL_SIZE / 2.0f);
  DrawGrid(100, VOXEL_SIZE);
  rlPopMatrix();
  // Draw coordinate system
  DrawCylinderEx(
      (Vector3){-VOXEL_SIZE / 2, (float)N_VOXEL_Y + 2, -VOXEL_SIZE / 2},
      (Vector3){50.0f, (float)N_VOXEL_Y + 2, 0.0f}, 0.01f, 0.01f, 24, RED);
  DrawCylinderEx((Vector3){-VOXEL_SIZE / 2, 0.0f, -VOXEL_SIZE / 2},
                 (Vector3){0.0f, 50.0f, 0.0f}, 0.01f, 0.01f, 24, GREEN);
  DrawCylinderEx(
      (Vector3){-VOXEL_SIZE / 2, (float)N_VOXEL_Y + 2, -VOXEL_SIZE / 2},
      (Vector3){0.0f, (float)N_VOXEL_Y + 2, 50.0f}, 0.01f, 0.01f, 24, BLUE);
  EndPerformanceTracker("Draw 3D debug information");
}
void Draw2DDebugInformation(int screen_width, int screen_height,
                            Chunk *chunk_data, Player *player) {
  StartPerformanceTracker("Draw 2D debug information");

  Chunk current_chunk = chunk_data[GetIndexCurrentChunk(player)];
  DrawText("X-Axis", screen_width - 40, 30, 10, RED);
  DrawText("Y-Axis", screen_width - 40, 40, 10, GREEN);
  DrawText("Z-Axis", screen_width - 40, 50, 10, BLUE);
  DrawText(
      TextFormat("Player location\n Voxel:  x=%i z=%i y=%i\n Exact: x=%.1f "
                 "z=%.1f y=%.1f\n Chunk:: x=%d z=%d",
                 player->body.voxel_position.x, player->body.voxel_position.z,
                 player->body.voxel_position.y, player->body.position.x,
                 player->body.position.z, player->body.position.y,
                 current_chunk.position.x, current_chunk.position.z),
      screen_width - 200, 30, 10, BLACK);

  // Print debug events in the following MAROON draw text function
  DrawDebugMessages();

  EndPerformanceTracker("Draw 2D debug information");
}
