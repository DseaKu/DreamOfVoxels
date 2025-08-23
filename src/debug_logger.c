#include "debug_logger.h"
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
  if (GetTime() - debug_message_set_time < 2.0) {
    DrawText("Debug text:", 30, 20, 10, MAROON);
    DrawText(debug_message, 30, 40, 10, MAROON);
  }
}
