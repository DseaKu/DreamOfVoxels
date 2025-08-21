#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include "std_includes.h"

//----------------------------------------------------------------------------------
// Keyboard inputs
//----------------------------------------------------------------------------------
typedef enum {
  NONE_INPUT = 1 << 0,

  KEY_INPUT_A_PRESSED = 1 << 1,
  KEY_INPUT_W_PRESSED = 1 << 2,
  KEY_INPUT_S_PRESSED = 1 << 3,
  KEY_INPUT_D_PRESSED = 1 << 4,
  KEY_INPUT_SPACE_PRESSED = 1 << 5,
} KeyboardInputsPressed;
typedef enum {
  LEFT_MOUSE_PRESSED = 1 << 0,

} MouseInputsPressed;

typedef struct InputFlags {

  u32 keyboard_pressed;
  u8 mouse_pressed;
} InputFlags;

#endif // INPUT_HANDLER_H
