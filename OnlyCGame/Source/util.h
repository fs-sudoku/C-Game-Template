#pragma once
#include <stdbool.h>
#include <cassert>
#include <malloc.h>
#include <stdlib.h>

#include <SDL_thread.h>

/* for verify calls */
typedef int print_type;

extern void M_ShowError(const char* const, const char* const);
extern void M_ShowVerifyError(int, const char* const, const char* const, const char*);
extern void D_Printf(const char* const, print_type, ...);

/* verify format const for __PrintVerifyFail */
#define __VerifyErrorFormat ("Verify failed!\n[+] Line: %d\n[+] File: %s\n[+] Function: %s\n[+] Condition: %s")

/* prints verify fail */
#define __PrintVerifyFail(file, line, func, condition) D_Printf(__VerifyErrorFormat, 1, line, file, func, condition), \
													   M_ShowVerifyError(line, file, func, condition)

/* throw error if (condition) == false */
#define UTIL_Verify(condition) if (!(condition))	{ __PrintVerifyFail(__FILE__, __LINE__, __FUNCSIG__, (#condition)); exit(-1); }

#define UTIL_CreateList(list)						(list) = malloc(sizeof(Node)); UTIL_Verify((list) != 0); (list)->value = 0; (list)->next = 0

#define UTIL_FillArray(arr, count, value)			for (size_t i = 0; i < (count); i++) { arr[i] = value; }

#define UTIL_SumVector(result, vector1, vector2)	UTIL_SetVector(result, (vector1.x + vector2.x), (vector1.y + vector2.y))
#define UTIL_SubVector(result, vector1, vector2)	UTIL_SetVector(result, (vector1.x - vector2.x), (vector1.y - vector2.y))
#define UTIL_ModVector(result, vector1, vector2)	UTIL_SetVector(result, (vector1.x * vector2.x), (vector1.y * vector2.y))

#define UTIL_SetVector(vector, xv, yv)				(vector).x = xv; (vector).y = yv
#define UTIL_SetRect(rect, xv, yv, wv, hw)			(rect).x = xv; (rect).y = yv; (rect).width = wv; (rect).height = hw;

#define UTIL_Name(x)							#x
#define UTIL_Random(rmax)						rand() % rmax
#define UTIL_SetSeed(seedmode)					time_t tm = time(0) + (time_t)abs(seedmode); srand(&tm)

/* sdl thread fast pimpl */
#define UTIL_ThreadID							SDL_ThreadID()
#define UTIL_CreateThread(func, data)			SDL_CreateThread(func, #func#data, data)
#define UTIL_WaitThread(index)					SDL_WaitThread(index, 0)
#define UTIL_WaitMS(ms)							SDL_Delay(ms)
/* base typedefs */

typedef short  int16;
typedef char   int8;

typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;

typedef struct SDL_Thread* Thread;

typedef struct
{
	int x, y;
} Vector2;

typedef struct
{
	int x, y;
} Vector2Int;

typedef struct
{
	uint16 x, y;
} SizeInt16;

typedef struct
{
} RectInt16;

typedef struct Node
{
	void*			value;
	struct Node*	next;
} Node;

void UTIL_PushInList(Node*, void*);