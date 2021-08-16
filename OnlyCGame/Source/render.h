#pragma once
#include "util.h"

#define SCREEN_WIDTH	(1366)
#define SCREEN_HEIGHT	(728)

typedef struct
{
	int r, g, b;
} Color;

typedef struct
{
	SizeInt16	size;
	char		name[32];
	void*		data;
} Texture;

typedef struct
{
	char		name[32];
	void*		data;
} Font;

typedef struct
{
	Font*		font;
	Vector2		position;
	SizeInt16	size;

	uint8		scale;
	char*		string;
	void*		render_data;
	void*		font_data;
} GUI_Text;

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

	bool		draw;
	bool		use_modify_color;
	Color		modify_color;
} Sprite;

extern Font*			Font_Load(const char* const);

extern GUI_Text*		GUI_Text_Create(Font*);
extern void				GUI_Text_SetString(GUI_Text*, const char* const);
extern void				GUI_Text_Draw(GUI_Text*);

extern Texture*			Texture_Load(const char* const);

extern Atlas*			Atlas_Create(Texture*, uint16, uint16);
extern RectInt16		Atlas_GetRectByIndex(Atlas*, uint16);
extern RectInt16		Atlas_GetRectByXY(Atlas*, uint16, uint16);
extern RectInt16		Atlas_GetRectByXYRect(Atlas* atlas, RectInt16 rect);

extern Sprite*			Sprite_Create(Texture*);
extern Sprite*			Sprite_CreateFromAtlas(Atlas*, RectInt16);

extern void				Sprite_Draw(Sprite*);
extern void				Sprite_SetColorModify(Sprite*, Color);
extern void				Sprite_ResetColorModify(Sprite*);
extern bool				Sprite_Contains(Sprite*, Vector2);