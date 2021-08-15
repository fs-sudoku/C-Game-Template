#include <SDL.h>
#include <SDL_image.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "game-world.h"

#define MAIN_SUB_VOID(name) inline static void (name)

#define MAIN_EXIT_SUCCESS	(+1)
#define MAIN_EXIT_FAILURE	(-1)

#define SDL_INIT_FLAGS		(SDL_INIT_VIDEO)
#define IMG_INIT_FLAGS		(IMG_INIT_PNG)

#define SDL_WINDOW_FLAGS	(SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN)
#define SDL_RENDERER_FLAGS	(SDL_RENDERER_PRESENTVSYNC)

#define SDL_RENDERER_INDEX  (-1)

const struct WindowParams {
	char title[16];
	uint16 width, height;
} main_window_param = {"Only C-SDL Game", 1366, 728};

struct SDL_Window*		main_window;
struct SDL_Renderer*	main_renderer;

bool					pressed_keys[SDL_NUM_SCANCODES];
double					delta_time = 0;

static Uint64			tm_now = 0;
static Uint64			tm_last = 0;

clock_t					global_ticks;

/* for verify in util.h */
void M_ShowVerifyError(int line, const char* const file, const char* const func, const char* condition)
{
	char title_buff[256], text_buff[512];
	sprintf(&title_buff, "Verify failed in %s", file);
	sprintf(&text_buff,  __VerifyErrorFormat, line, file, func, condition);

	M_ShowError(title_buff, text_buff);
}

void M_ShowError(const char* const title, const char* const text)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, text, NULL);
}

extern bool I_IsKeyPressed(uint32 scancode)
{
	return pressed_keys[scancode];
}

MAIN_SUB_VOID(M_InitSubsystem())
{
	global_ticks = clock();
	time_t tm = time(0);

	for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
		pressed_keys[i] = false;
	}

	UTIL_Verify(SDL_Init(SDL_INIT_EVERYTHING) != true);
	UTIL_Verify(IMG_Init(IMG_INIT_FLAGS) == IMG_INIT_FLAGS);
}

MAIN_SUB_VOID(M_CreateWindow())
{
	main_window = SDL_CreateWindow(
		main_window_param.title, 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		main_window_param.width,
		main_window_param.height,
		SDL_WINDOW_FLAGS
	);
	main_renderer = SDL_CreateRenderer(
		main_window,
		SDL_RENDERER_INDEX,
		SDL_RENDERER_FLAGS
	);

	UTIL_Verify(main_window != 0);
	UTIL_Verify(main_renderer != 0);
}

MAIN_SUB_VOID(M_StartLoop())
{
	tm_now = SDL_GetPerformanceCounter();

	SDL_SetRenderDrawColor(main_renderer, 70, 70, 70, 255);

	while (true)
	{
		SDL_Event window_event;
		while (SDL_PollEvent(&window_event) != false)
		{
			switch (window_event.type)
			{
			case SDL_QUIT:
				return;
			case SDL_KEYUP:
				pressed_keys[window_event.key.keysym.scancode] = false;
				break;
			case SDL_KEYDOWN:
				pressed_keys[window_event.key.keysym.scancode] = true;
				break;
			default:
				break;
			}
			/* send event to game-world */
			W_InputGameWorld(&window_event);
		}
		SDL_RenderClear(main_renderer);

		/* calculate delta time */
		tm_last = tm_now;
		tm_now = SDL_GetPerformanceCounter();

		delta_time = (double)((tm_now - tm_last) * 1000 / (double)SDL_GetPerformanceFrequency());
		/* update game-world */
		W_UpdateGameWorld();

		SDL_RenderPresent(main_renderer);
	}
}

MAIN_SUB_VOID(M_Exit())
{
	D_Printf("Total game life time: %f", info, (double)(clock() - global_ticks) / CLOCKS_PER_SEC);

	SDL_DestroyWindow(main_window);
	SDL_DestroyRenderer(main_renderer);

	malloc(-1);

	SDL_Quit(), IMG_Quit();
}

int main(int argc, char** argv)
{
	/* step 1 - app arguments */
	D_Printf("Game started with (%d) arguments", info, argc);
	for (size_t i = 0; i < argc; i++)
	{
		D_Printf("+ %d argument: %s", info, i+1, argv[i]);
	}
	/* step 2 - prepare sdl */
	M_InitSubsystem();
	/* step 3 - create window */
	M_CreateWindow();
	/* step 4 - create game world */
	W_CreateGameWorld();
	/* step 5 - start loop */
	M_StartLoop();
	/* step 6 - full clear game world */
	W_ClearGameWorld(true);
	/* step 7 - exit */
	M_Exit();
	return MAIN_EXIT_SUCCESS;
}