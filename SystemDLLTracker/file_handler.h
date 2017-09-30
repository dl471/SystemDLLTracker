#pragma once

#define INVALID_ELEMENT 	-1
#define END_OF_BUFFER		-2

#define FILE_IS_EMPTY		-1

// all the magic return codes in file_handler.cpp will over time be replaced with meaningful names and handled. i think working with microsoft macros have instilled in me a burning need to #define.

#define DEFAULT_MAX_FILE_SIZE     0x400000   // 2 to the power of 22, just over 4MB, should be enough for all purposes
#define EXTENDED_MAX_FILE_SIZE    0x7a12000  // if 4MB is not enough the plugin will attempt to allocate more memory up until a maximum of 128 MB, which is considered exceptionally unrealistic

#ifndef _MAIN_HEADER
#include "main_header.h"
#define _MAIN_HEADER
#endif

#ifndef _ERROR_HANDLER
#define _ERROR_HANDLER
#include "error_handler.h"
#endif

class FileHandler {

public:
	FileHandler();
	~FileHandler();
	int OpenFile(int mode);
	int CloseFile();
	int CommitToFile(char *short_name, char *full_name, BOOL is_system_dll);
	int ReadFile();
	t_module *FirstRoundValidate(); // misleading name

protected:	
	int FindElement(char *data_buffer, char *element, int element_size, int trailing_size, int *buffer_pos, int buffer_size);
	int GetFileSize();

	FILE *data_file;

	int file_size;
	char *data_buffer;
	int fsl; // file seek location

};