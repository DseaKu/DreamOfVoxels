#include "input_handler.h"
#include "resource_tracker.h"
#include <raylib.h>

InputData InitInputsFlags() {
  InputData input_flags = {0};
  return input_flags;
}
void UpdateInputs(InputData *input_flags) {
  StartPerformanceTracker("Check for inputs");
  //----------------------------------------------------------------------------------
  // Keyboard inputs
  //----------------------------------------------------------------------------------
  input_flags->keyboard_pressed = 0;
  if (IsKeyDown(IsKeyDown(KEY_W))) {
    input_flags->keyboard_pressed |= KEY_INPUT_W_PRESSED;
  }
  if (IsKeyDown(IsKeyDown(KEY_A))) {
    input_flags->keyboard_pressed |= KEY_INPUT_A_PRESSED;
  }
  if (IsKeyDown(IsKeyDown(KEY_S))) {
    input_flags->keyboard_pressed |= KEY_INPUT_S_PRESSED;
  }
  if (IsKeyDown(IsKeyDown(KEY_D))) {
    input_flags->keyboard_pressed |= KEY_INPUT_D_PRESSED;
  }
  if (IsKeyDown(IsKeyDown(KEY_SPACE))) {
    input_flags->keyboard_pressed |= KEY_INPUT_SPACE_PRESSED;
  }
  if (IsKeyDown(IsKeyDown(KEY_LEFT_CONTROL))) {
    input_flags->keyboard_pressed |= KEY_INPUT_CTRL_PRESSED;
  }
  //----------------------------------------------------------------------------------
  // Mouse inputs and position
  //----------------------------------------------------------------------------------
  input_flags->mouse_pressed = 0;
  input_flags->mouse_delta = GetMouseDelta();
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    input_flags->mouse_pressed |= LEFT_MOUSE_PRESSED;
  }
  EndPerformanceTracker("Check for inputs");
}
