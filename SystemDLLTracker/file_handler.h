#pragma once

#define INVALID_ELEMENT 	-1
#define END_OF_BUFFER		-2

#define FILE_IS_EMPTY		-1

// all the magic return codes in file_handler.cpp will over time be replaced with meaningful names and handled. i think working with microsoft macros have instilled in me a burning need to #define.

#define MAX_FILE_SIZE 4194304 // 2 to the power of 22, just over 4MB, should be enough for all purposes

#ifndef SDT_MAIN_HEADER
#include "main_header.h"
#define SDT_MAIN_HEADER
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