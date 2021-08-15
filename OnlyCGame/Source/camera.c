#include <SDL_scancode.h>

#include "input.h"
#include "camera.h"

extern double delta_time;

Camera2D* Cam_Create()
{
	Camera2D* result = malloc(sizeof(Camera2D));
	UTIL_Verify(result != 0);

	UTIL_SetVector(result->position, 0, 0);

	return result;
}

void Cam_Update(Camera2D* cam)
{
	Vector2 dir = {0, 0};
	float calc_speed = delta_time * CAMERA_SPEED;
	if (I_IsKeyPressed(SDL_SCANCODE_W)) {
		dir.y += calc_speed;
	}
	if (I_IsKeyPressed(SDL_SCANCODE_S)) {
		dir.y -= calc_speed;
	}
	if (I_IsKeyPressed(SDL_SCANCODE_A)) {
		dir.x += calc_speed;
	}
	if (I_IsKeyPressed(SDL_SCANCODE_D)) {
		dir.x -= calc_speed;
	}
	UTIL_SumVector(cam->position, cam->position, dir);
}