#include <SDL_events.h>

#include "game-control.h"
#include "game-world.h"
#include "render.h"
#include "debug.h"
#include "input.h"

#define TILE_RECT(x, y) { (uint16)(x), (uint16)(y), (uint16)1, (uint16)1 }

Sprite*			w_single_sprites[WORLD_MAX_SPRITE_COUNT];
Camera2D*		w_camera;
Atlas*			w_ground_atlas;
Texture*		w_ground_texture;

uint16		w_single_sprites_count = 0;
uint16		w_collide_tiles_a_density = 15;

Vector2	w_mouse_pos;

static RectInt16 w_ground_tiles[] =
{
	TILE_RECT(0, 0), TILE_RECT(0, 0), TILE_RECT(1, 0), 
	TILE_RECT(2, 0), TILE_RECT(3, 0), TILE_RECT(4, 0), 
	TILE_RECT(5, 0), TILE_RECT(6, 0), TILE_RECT(7, 0),
};

static RectInt16 w_collide_tiles[] =
{
	TILE_RECT(4, 2), TILE_RECT(5, 2), TILE_RECT(6, 2)
};

static uint16 w_ground_tiles_length = sizeof(w_ground_tiles) / sizeof(RectInt16);
static uint16 w_collide_tiles_length = sizeof(w_collide_tiles) / sizeof(RectInt16);

static void W_CreateCamera()
{
	w_camera = Cam_Create();
}

static RectInt16 W_RandomTile()
{
	return Atlas_GetRectByXYRect(w_ground_atlas, 
		UTIL_Random(w_collide_tiles_a_density) ? 
			w_ground_tiles[UTIL_Random(w_ground_tiles_length)] :
			w_collide_tiles[UTIL_Random(w_collide_tiles_length)]
	);
}

static void W_Thread_PlaceTiles(int i_offset)
{
	SizeInt16 offset;
	uint8 i_offset_int = *((uint8*)i_offset);

	UTIL_SetSeed(UTIL_ThreadID);

	UTIL_SetVector(offset,
		i_offset * (WORLD_TILE_SIZE_X / 2),
		i_offset * (WORLD_TILE_SIZE_Y / 2)
	);
	for (size_t i = 0; i < WORLD_SIZE_X; i++)
	{
		for (size_t x = 0; x < WORLD_SIZE_Y; x++)
		{
			Sprite* sprite = Sprite_Create(w_ground_texture);

			UTIL_SetVector(sprite->position,
				offset.x + i * WORLD_TILE_SIZE_X,
				offset.y + x * WORLD_TILE_SIZE_Y
			);
			sprite->rect = W_RandomTile();
		}
	}
}

static void W_GenerateMap()
{
	w_ground_texture = Texture_Load("Data/seasons_tiles.png");
	w_ground_atlas = Atlas_Create(w_ground_texture, 8, 12);

	for (uint8 i = 0; i < 2; i++)
	{
		W_Thread_PlaceTiles(i);
	}

	UTIL_SetVector(w_camera->position, 
		(WORLD_SIZE_X / 2) * -WORLD_TILE_SIZE_X,
		(WORLD_SIZE_Y / 2) * -WORLD_TILE_SIZE_Y
	);
}

inline static void W_ControlCamera()
{
	Cam_Update(w_camera);
}

void W_Create()
{
	UTIL_FillArray(w_single_sprites, WORLD_MAX_SPRITE_COUNT, 0);

	W_CreateCamera();
	W_GenerateMap();

	C_Init();
}

void W_Update()
{
	for (size_t i = 0; i < w_single_sprites_count; i++)
	{
		if (w_single_sprites[i] != 0) {
			Sprite_Draw(w_single_sprites[i]);
		}
	}
	W_ControlCamera();
	/* update mouse pos */
	UTIL_SetVector(w_mouse_pos,
		i_mouse_pos.x - w_camera->position.x,
		i_mouse_pos.y - w_camera->position.y
	);
	C_Update();
}

void W_Input(SDL_Event* input)
{
	C_Input(input);
}

void W_Clear(bool is_final)
{
	for (size_t i = 0; i < WORLD_MAX_SPRITE_COUNT; i++)
	{
		if (w_single_sprites[i] != 0) {
			free(w_single_sprites[i]);
		}
	}
	if (is_final) {
		C_Release();
	}
}

void W_RegisterSingleSprite(Sprite* sprite)
{
	for (size_t i = 0; i < WORLD_MAX_SPRITE_COUNT; i++)
	{
		if (w_single_sprites[i] == 0) {
			w_single_sprites[i] = sprite;
			w_single_sprites_count++;
			return;
		}
	}
}

void W_UnRegisterSingleSprite(Sprite* sprite)
{
	for (size_t i = 0; i < WORLD_MAX_SPRITE_COUNT; i++)
	{
		if (w_single_sprites[i] == sprite) {
			w_single_sprites[i] = 0;
			w_single_sprites_count--;
			return;
		}
	}
}