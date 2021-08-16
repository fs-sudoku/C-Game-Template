#include "render.h"
#include "game-world.h"
#include "debug.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string.h>
#include <stdio.h>

#define LOAD_TEXTURE_FAIL	(-1)

#define NULL_STR			("")
#define SPACE_STR			(" ")

extern Camera2D* w_camera;
extern SDL_Renderer* main_renderer;

static const Color white_color = { 0, 0, 0 };

Font* Font_Load(const char* const path)
{
	Font* result = malloc(sizeof(Font));
	UTIL_Verify(result != 0);

	result->data = TTF_OpenFont(path, 20);
	UTIL_Verify(result->data != 0);

	return result;
}

GUI_Text* GUI_Text_Create(Font* font)
{
	GUI_Text* result = malloc(sizeof(GUI_Text));
	UTIL_Verify(result != 0);
	UTIL_SetVector(result->position, 0, 0);

	result->font = font;
	result->scale = 1;

	GUI_Text_SetString(result, NULL_STR);

	return result;
}

void GUI_Text_SetString(GUI_Text* text, const char* const string)
{
	const char* const final_str = string == NULL_STR ? SPACE_STR : string;

	SDL_Color color_ptr = { 255, 255, 255, 255 };
	SDL_Surface* srf = TTF_RenderUTF8_Blended_Wrapped(text->font->data, final_str, color_ptr, 1024);
	UTIL_Verify(srf != 0);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(main_renderer, srf);
	UTIL_Verify(texture != 0);

	UTIL_SetVector(text->size, srf->w, srf->h);

	text->render_data = texture;
	text->string = string;

	SDL_FreeSurface(srf);
}

void GUI_Text_Draw(GUI_Text* text)
{
	SDL_Rect rect = {
		text->position.x, text->position.y,
		text->size.x * text->scale,
		text->size.y * text->scale
	};

	SDL_RenderCopy(main_renderer, text->render_data, NULL, &rect);
}

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
	result->draw = true;
	result->use_modify_color = false;
	result->modify_color = white_color;

	W_RegisterSingleSprite(result);

	return result;
}

Sprite* Sprite_CreateFromAtlas(Atlas* atlas, Rect rect)
{
	Sprite* result = Sprite_Create(atlas->texture);
	result->rect = rect;

	return result;
}

void Sprite_Draw(Sprite* target)
{
	if (!target->draw) return;

	UTIL_Verify(target);

	SDL_Rect rect = { 
		(target->position.x + w_camera->position.x) * w_camera->zoom,
		(target->position.y + w_camera->position.y) * w_camera->zoom,
		target->rect.width * target->scale.x * w_camera->zoom,
		target->rect.height * target->scale.y * w_camera->zoom
	};
	SDL_Rect clip = { 
		target->rect.x, target->rect.y,
		target->rect.width, target->rect.height 
	};

	if (target->use_modify_color) {
		SDL_SetTextureColorMod(target->texture->data,
			target->modify_color.r,
			target->modify_color.g,
			target->modify_color.b
		);
		SDL_RenderCopy(main_renderer, target->texture->data, &clip, &rect);
		SDL_SetTextureColorMod(target->texture->data, 255, 255, 255);
	}
	else {
		SDL_RenderCopy(main_renderer, target->texture->data, &clip, &rect);
	}
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

	result->rect_array = malloc(result->elements_count * sizeof(Rect));
	UTIL_Verify(result->rect_array != 0);
	
	return result;
}

Rect Atlas_GetRectByIndex(Atlas* atlas, uint16 index)
{
	UTIL_Verify(false);
	// TODO
}

Rect Atlas_GetRectByXYRect(Atlas* atlas, Rect rect)
{
	Rect result;
	int16 x_per_p = atlas->element_per_pixel.x, y_per_p = atlas->element_per_pixel.y;

	UTIL_SetRect(result, rect.x * x_per_p, rect.y * y_per_p, x_per_p, y_per_p);
	return result;
}

Rect Atlas_GetRectByXY(Atlas* atlas, uint16 index_x, uint16 index_y)
{
	Rect result;
	int16 x_per_p = atlas->element_per_pixel.x, y_per_p = atlas->element_per_pixel.y;

	UTIL_SetRect(result, index_x * x_per_p, index_y * y_per_p, x_per_p, y_per_p);
	return result;
}

bool Sprite_Contains(Sprite* sprite, Vector2 point)
{
	SDL_Point p = { point.x, point.y };
	SDL_Rect r = { sprite->position.x, sprite->position.y, sprite->rect.width, sprite->rect.height };
	return SDL_PointInRect(&p, &r);
}

void Sprite_SetColorModify(Sprite* sprite, Color color)
{
	sprite->use_modify_color = true;
	sprite->modify_color = color;
}

void Sprite_ResetColorModify(Sprite* sprite)
{
	sprite->use_modify_color = false;
	sprite->modify_color = white_color;
}