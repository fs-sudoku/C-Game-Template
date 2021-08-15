#include "render.h"
#include "game-world.h"
#include "debug.h"

#include <SDL_image.h>

#include <string.h>
#include <stdio.h>

#define LOAD_TEXTURE_FAIL (-1)

extern SDL_Renderer* main_renderer;

Texture* Texture_Load(const char* const path)
{
	UTIL_Verify(main_renderer != 0);

	Texture* result = malloc(sizeof(Texture));
	UTIL_Verify(result != 0);

	result->data = IMG_LoadTexture(main_renderer, path);
	UTIL_Verify(result->data != 0);

	strcpy(result->name, path);
	
	uint32 format;
	int w, h, access;
	SDL_QueryTexture(result->data, &format, &access, &w, &h);

	SizeInt16 size = { w, h };
	result->size = size;

	return result;
}

Sprite* Sprite_Create(Texture* texture)
{
	UTIL_Verify(texture != 0);

	Sprite* result = malloc(sizeof(Sprite));
	UTIL_Verify(result != 0);

	UTIL_SetVector(result->position, 0, 0);
	UTIL_SetVector(result->scale, 1, 1);
	UTIL_SetRect(result->rect, 0, 0, texture->size.x, texture->size.y);
	
	result->texture	= texture;

	W_RegisterSingleSprite(result);

	return result;
}

Sprite* Sprite_CreateFromAtlas(Atlas* atlas, RectInt16 rect)
{
	Sprite* result = Sprite_Create(atlas->texture);
	result->rect = rect;

	return result;
}

void Sprite_Draw(Sprite* target, Vector2Int cam_pos)
{
	UTIL_Verify(target);

	SDL_Rect rect = { 
		target->position.x + cam_pos.x, target->position.y + cam_pos.y,
		target->rect.width * target->scale.x,
		target->rect.height * target->scale.y
	};
	SDL_Rect clip = { 
		target->rect.x, target->rect.y,
		target->rect.width, target->rect.height 
	};

	SDL_RenderCopy(main_renderer, target->texture->data, &clip, &rect);
}

Atlas* Atlas_Create(Texture* texture, uint16 count_x, uint16 count_y)
{
	UTIL_Verify(texture != 0);

	Atlas* result = malloc(sizeof(Atlas));
	UTIL_Verify(result != 0);

	result->texture = texture;
	result->elements_count = count_x * count_y;
	UTIL_SetVector(result->elements_size, count_x, count_y);
	UTIL_SetVector(result->element_per_pixel, texture->size.x / count_x, texture->size.y / count_y);

	result->rect_array = malloc(result->elements_count * sizeof(RectInt16));
	UTIL_Verify(result->rect_array != 0);
	
	return result;
}

RectInt16 Atlas_GetRectByIndex(Atlas* atlas, uint16 index)
{
	UTIL_Verify(false);
	// TODO
}

RectInt16 Atlas_GetRectByXYRect(Atlas* atlas, RectInt16 rect)
{
	RectInt16 result;
	int16 x_per_p = atlas->element_per_pixel.x, y_per_p = atlas->element_per_pixel.y;

	UTIL_SetRect(result, rect.x * x_per_p, rect.y * y_per_p, x_per_p, y_per_p);
	return result;
}

RectInt16 Atlas_GetRectByXY(Atlas* atlas, uint16 index_x, uint16 index_y)
{
	RectInt16 result;
	int16 x_per_p = atlas->element_per_pixel.x, y_per_p = atlas->element_per_pixel.y;

	UTIL_SetRect(result, index_x * x_per_p, index_y * y_per_p, x_per_p, y_per_p);
	return result;
}