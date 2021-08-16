#include "game-ui.h"
#include "game-world.h"
#include "input.h"

#include <stdio.h>

/* from game-world */
extern uint16		w_single_sprites_count;
extern Camera2D*	w_camera;

Font* u_arial_font;

static GUI_Text* u_debug_text;

void G_Init()
{
	u_arial_font = Font_Load("Data/arial.ttf");
	u_debug_text = GUI_Text_Create(u_arial_font);
}

void G_Update()
{
	char debug_text_buff[128];
	sprintf(debug_text_buff, 
		"Mouse screen position: X:%d Y:%d\nMouse world position: X:%d Y:%d\nSprite count in scene: %u", 
		i_mouse_pos.x, i_mouse_pos.y, w_mouse_pos.x, w_mouse_pos.y,
		w_single_sprites_count
	);
	GUI_Text_SetString(u_debug_text, debug_text_buff);
	GUI_Text_Draw(u_debug_text);
}

void G_Release()
{

}