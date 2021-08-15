#pragma once
#include "util.h"

typedef struct
{
	SizeInt16	size;
	char		name[32];
	void*		data;
} Texture;

typedef struct
{
	Texture*	texture;
	SizeInt16	elements_size;
	RectInt16*	rect_array;

	uint16		elements_count;
	SizeInt16	element_per_pixel;
} Atlas;

typedef struct
{
	Vector2		position;
	RectInt16	rect;
	SizeInt16	scale;
	Texture*	texture;
} Sprite;

extern Texture*			Texture_Load(const char* const);

extern Atlas*			Atlas_Create(Texture*, uint16, uint16);
extern RectInt16		Atlas_GetRectByIndex(Atlas*, uint16);
extern RectInt16		Atlas_GetRectByXY(Atlas*, uint16, uint16);
extern RectInt16		Atlas_GetRectByXYRect(Atlas* atlas, RectInt16 rect);

extern Sprite*			Sprite_Create(Texture*);
extern Sprite*			Sprite_CreateFromAtlas(Atlas*, RectInt16);

extern void				Sprite_Draw(Sprite*, Vector2Int);