#pragma once

#include <windows.h>

#include <stdio.h>
#include <string.h>

#ifndef _PLUGIN
#define _PLUGIN
#include "Plugin.h"
#endif

#define NOT_PRESENT				0 
#define PRESENT_UP_TO_DATE		1 // means there is a node with matching path already loaded and the data in odbg's module table is the most up to date, so the nodes in module_list will be updated to reflect this
#define PRESENT_MARK_SYSTEM		2 // means there is a node with matching path already loaded and the data loaded from file is the most up to date, so the data in odbg's module table will be updated to reflect this
#define PRESENT_MARK_NON_SYSTEM	3 // same as above

#define FROM_LIVE_DATA			0
#define FROM_FILE_RECORD		1

#define READ					0
#define WRITE					1

extern const char *project_name;
extern const char *project_save_file;