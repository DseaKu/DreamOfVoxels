#include "../../includes/sprites/player.h"

Player InitPlayer(void) {
    Player player = {0};
    player.camera.position = (Vector3){0.0f, 2.0f, 4.0f};
    player.camera.target = (Vector3){0.0f, 1.8f, 0.0f};
    player.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    player.camera.fovy = 60.0f;
    player.camera.projection = CAMERA_PERSPECTIVE;
    player.speed = 0.1f;
    return player;
}

void UpdatePlayer(Player *player) {
    UpdateCamera(&player->camera, CAMERA_FIRST_PERSON);
}