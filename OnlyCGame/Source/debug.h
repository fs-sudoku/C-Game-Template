#pragma once
typedef int print_type;

typedef enum { info, error, warning } print_type_enum;

extern void D_Printf(const char* const, print_type, ...);