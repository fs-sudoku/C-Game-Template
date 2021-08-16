#pragma once
#include "camera.h"
#include "render.h"

#define WORLD_MAX_SPRITE_COUNT	(1024 * 1024)
#define WORLD_SIZE_X			(64)
#define WORLD_SIZE_Y			(64)
#define WORLD_TILE_SIZE_X		(128)
#define WORLD_TILE_SIZE_Y		(64)

extern Sprite*			w_single_sprites[WORLD_MAX_SPRITE_COUNT];

extern Atlas*			w_ground_atlas;
extern Camera2D*		w_camera;
extern Texture*			w_ground_texture;

extern Vector2		w_mouse_pos;

extern void W_Create();
extern void W_Update();
extern void W_Input(union SDL_Event*);

extern void W_Clear(bool);

extern void W_RegisterSingleSprite(struct Sprite*);
extern void W_UnRegisterSingleSprite(struct Sprite*);