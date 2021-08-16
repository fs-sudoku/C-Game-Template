#pragma once
#include "util.h"

#define CAMERA_SPEED (0.5)

typedef struct
{
	Vector2 position;
	float zoom;
} Camera2D;

Camera2D* Cam_Create();

void Cam_Update(Camera2D*);