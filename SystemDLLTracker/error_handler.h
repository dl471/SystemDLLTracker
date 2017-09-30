#pragma once

#ifndef _MAIN_HEADER
#include "main_header.h"
#define _MAIN_HEADER
#endif

#define EXCEEDED_MAX_FILE_SIZE	  -4
#define MALLOC_ERROR              -5
#define FILE_OPEN_FAILURE         -6
#define FILE_CLOSE_FAILURE        -7
#define UNDEFINED_FILE_ERROR      -8
#define GFSE_ERROR                -9
#define CLOSE_HANDLE_ERROR        -10
#define FILE_READ_ERROR           -11
#define LOST_MODULE_LIST          -12

#define ERROR_RECOVERABLE          0x00
#define ERROR_IRRECOVERABLE        0x01

extern void HandleError(int error, int severity);

extern int plugin_halted;