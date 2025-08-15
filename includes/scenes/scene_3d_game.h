#ifndef SCENE_3D_GAME_H
#define SCENE_3D_GAME_H

#define MAX_SCREEN_WIDTH 1400
#define MAX_SCREEN_HEIGHT 900
#define CURSOR_RADIUS 2

#define TARGET_FPS 3000
#define BLOCK_SIZE 1.0f
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2.0f)
#define MAX_PLACMENT_DISTANCE 1000.0

int Scene3DGame();
void Draw3DDebugInformation(int screen_width, int screen_height);
void Draw2DDebugInformation(int screen_width, int screen_height);

#endif // SCENE_3D_GAME_H
