#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H
#include "std_includes.h"
#include <raylib.h>

//----------------------------------------------------------------------------------
// Keyboard inputs
//----------------------------------------------------------------------------------
typedef enum {
  NO_KEY_PRESSED = 1 << 0,

  KEY_INPUT_W_PRESSED = 1 << 1,
  KEY_INPUT_A_PRESSED = 1 << 2,
  KEY_INPUT_S_PRESSED = 1 << 3,
  KEY_INPUT_D_PRESSED = 1 << 4,
  KEY_INPUT_SPACE_PRESSED = 1 << 5,
  KEY_INPUT_CTRL_PRESSED = 1 << 6,
} KeyboardInputsPressed;

//----------------------------------------------------------------------------------
// Mouse inputs
//----------------------------------------------------------------------------------
typedef enum {

  NO_MOUSE_PRESSED = 1 << 0,
  LEFT_MOUSE_PRESSED = 1 << 1,
  RIGHT_MOUSE_PRESSED = 1 << 2,

} MouseInputsPressed;

typedef struct InputData {

  u16 keyboard_pressed;
  u8 mouse_pressed;
  Vector2 mouse_delta;
} InputData;

InputData InitInputsFlags();
void GetInputs(InputData *input_data);

#endif // INPUT_HANDLER_H
