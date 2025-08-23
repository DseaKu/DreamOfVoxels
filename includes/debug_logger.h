#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <raylib.h>

extern char debug_message[256];
void SetDebugMessage(const char *message, ...);
void DrawDebugMessages(void);

#endif // DEBUG_LOGGER_H
