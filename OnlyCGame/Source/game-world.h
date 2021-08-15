#pragma once
#include "camera.h"
#include "render.h"

#define WORLD_MAX_SPRITE_COUNT	(4096)
#define WORLD_SIZE_X			(16)
#define WORLD_SIZE_Y			(16)
#define WORLD_TILE_SIZE_X		(128)
#define WORLD_TILE_SIZE_Y		(64)

extern Sprite*			w_single_sprites[WORLD_MAX_SPRITE_COUNT];

extern Atlas*			w_ground_atlas;
extern Camera2D*		w_camera;
extern Texture*			w_ground_texture;

extern void W_CreateGameWorld();
extern void W_UpdateGameWorld();
extern void W_InputGameWorld(union SDL_Event*);

extern void W_ClearGameWorld(bool);

extern void W_RegisterSingleSprite(struct Sprite*);
extern void W_UnRegisterSingleSprite(struct Sprite*);