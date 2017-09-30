#pragma once

#ifndef _MAIN_HEADER
#include "main_header.h"
#define _MAIN_HEADER
#endif

#ifndef _FILE_HANDLER
#define _FILE_HANDLER
#include "file_handler.h"
#endif

#ifndef _ERROR_HANDLER
#define _ERROR_HANDLER
#include "error_handler.h"
#endif

struct node {

	char short_name[SHORTLEN+1]; // the strings provided by ollydbg are not null terminated, so i will be doing for it. this is also why i will be using memmove instead of strcpy.
	char full_name[MAX_PATH+1];
	BOOL is_system_dll;
	BOOL last_update_from_file;

	node *prev;

};

class ModuleList {

public:
	ModuleList();
	~ModuleList();
	int AddNode(t_module *target, int last_update_from_file);
	int PresentInList(char *path);
	int UpdateIsSystemDLLInfo(char *path, int issystemdll);
	int MarkUpdated();

protected:
	int number_of_nodes;

	int ZeroStrings(node *new_node);
	node *SearchNodes(char *path);
	int FreeNodes();
	
	int SaveFile();
	int LoadFile();

	FileHandler *file_io;

	node *first;
	node *last;
	node *last_accessed; // storing last result from search nodes so functions that want to change node data but don't get direct access to search nodes or nodes data don't need to request another search

};