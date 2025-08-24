#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include "scene_voxel_game.h"
extern char debug_message[256];
void SetDebugMessage(const char *message, ...);
void DrawDebugMessages(void);

void Draw3DDebugInformation(int screen_width, int screen_height);
void Draw2DDebugInformation(int screen_width, int screen_height,
                            Chunk *chunk_data, Player *player);
#endif // DEBUG_LOGGER_H
