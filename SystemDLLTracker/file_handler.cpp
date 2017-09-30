#ifndef _FILE_HANDLER
#define _FILE_HANDLER
#include "file_handler.h"
#endif

FileHandler::FileHandler() {

	fsl = 0;
	data_file = NULL;

}

FileHandler::~FileHandler() {



}

int FileHandler::OpenFile(int mode) {

	int retcode;

	if (mode == READ) {

		retcode = fopen_s(&data_file, project_save_file, "r");

	}


	if (mode == WRITE) {

		retcode = fopen_s(&data_file, project_save_file, "w");

	}

	if (retcode != 0) {

		HandleError(FILE_OPEN_FAILURE, ERROR_IRRECOVERABLE);

	}

	return 0;

}

int FileHandler::CloseFile() {

	int retcode;

	retcode = fclose(data_file);

	if (retcode == EOF) {

		HandleError(FILE_CLOSE_FAILURE, ERROR_IRRECOVERABLE);

	}
	
	return 0;

}

int FileHandler::CommitToFile(char *short_name, char *full_name, BOOL is_system_dll){

	if (is_system_dll) {

		fprintf(data_file, "SHORT_NAME=%s;\nFULL_PATH=%s;\nIS_SYSTEM_DLL=YES;\n\n", short_name, full_name);

	}
	else {

		fprintf(data_file, "SHORT_NAME=%s;\nFULL_PATH=%s;\nIS_SYSTEM_DLL=NO;\n\n", short_name, full_name);

	}

	return 0;
}

int FileHandler::FindElement(char *data_buffer, char *element, int element_size, int trailing_size, int *buffer_pos, int buffer_size) {

	// find element could theoritically and reasonably be divided into find tag and check value, since it's getting very very nested and doing almost two jobs at once. however, i originally did this in part because i didn't want to have double the function calls and it at current lies firmly in the "if it ain't broke don't fix it" territory

	int i = *buffer_pos;
	int j;
	int valid = 0;

	while (i < buffer_size) {

		if (data_buffer[i] == element[0]) {

			if (memcmp(&data_buffer[i], element, element_size) == 0) {

				i += element_size;

				if (data_buffer[i] == 0x3B) {

					*buffer_pos = i;
					return INVALID_ELEMENT; // cannot start with semi-colon / was too short

				}

				for (j = 0; j < trailing_size + 1; j++) {

					if (data_buffer[i] == 0x3B) {

						*buffer_pos = i;
						return j;

					}

					i++;

				}

				*buffer_pos = i;
				return INVALID_ELEMENT; // name was too long

			}

		}

		i++;

	}

	return END_OF_BUFFER;

}

t_module *FileHandler::FirstRoundValidate() {
	
	// this was supposed to be a quick that the file format was valid, which rapidly balooned into this. i should change the name to something more accurate at some point.
	// this was mostly made as learning exercise because i like writing parsers and because a clear human readable and if desired human editable format was desired, consider using a more stable serialization format than "my own custom format" if it retains the same properties. JSON?

	int j = 0;
	int ret = 0;

	t_module temp;
	t_module *complete_node;

	while (1) {

		memset(temp.name, 0x00, SHORTLEN + 1); 
		memset(temp.path, 0x00, MAX_PATH + 1);
		temp.issystemdll = 0;

		ret = FindElement(data_buffer, "SHORT_NAME=", 11, SHORTLEN, &fsl, file_size);
		if (ret == END_OF_BUFFER)	{ break;	}
		if (ret == INVALID_ELEMENT) { continue; }

		memmove(temp.name, &data_buffer[fsl - ret], ret);
		fsl++; // to compensate for the missed i++ that results from retuning in the middle of the for loop in FindElement

		ret = FindElement(data_buffer, "FULL_PATH=", 10, MAX_PATH, &fsl, file_size);
		if (ret == END_OF_BUFFER)	{ break;	}
		if (ret == INVALID_ELEMENT) { continue; }

		memmove(temp.path, &data_buffer[fsl - ret], ret);
		fsl++;

		ret = FindElement(data_buffer, "IS_SYSTEM_DLL=", 14, 3, &fsl, file_size);
		if (ret == END_OF_BUFFER)	{ break;	}
		if (ret == INVALID_ELEMENT) { continue; }

		if (ret == 2) {

			if (memcmp(&data_buffer[fsl - ret], "NO", 2) == 0) {

				temp.issystemdll = 0;
				fsl++;

			}
			else {

				continue;

			}

		}

		if (ret == 3) {

			if (memcmp(&data_buffer[fsl - ret], "YES", 3) == 0) {

				temp.issystemdll = 1;
				fsl++;

			}
			else {

				continue;

			}

		}

		complete_node = new t_module;
		memmove(complete_node, &temp, sizeof(temp));

		return complete_node;

	}

	complete_node = NULL;
	return complete_node;

}

int FileHandler::GetFileSize() {

	int success = 0;
	HANDLE file;
	LARGE_INTEGER file_size;

	file = CreateFileA(project_save_file, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE) {

		DWORD error;
		error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND) {

			return ERROR_FILE_NOT_FOUND;

		}
		else {

			HandleError(UNDEFINED_FILE_ERROR, ERROR_IRRECOVERABLE);
			
			/*
			possible candidates for more extensive checking are:
			ERROR_ACCESS_DENIED
			ERROR_PATH_NOT_FOUND
			ERROR_TOO_MANY_OPEN_FILES
			*/

		}

	}

	success = GetFileSizeEx(file, &file_size);

	if (!success) {

		HandleError(GFSE_ERROR, ERROR_IRRECOVERABLE);

	}

	success = CloseHandle(file);

	if (!success) {

		HandleError(CLOSE_HANDLE_ERROR, ERROR_IRRECOVERABLE);

	}

	return file_size.QuadPart; // a plugin for a 32-bit debugger that does not run on 32-bit systems, how ironic
	// also getting warning about return type/size, will need to change filesizze etc. to long long
}

int FileHandler::ReadFile() {

	int success;
	
	data_buffer = (char *)malloc(DEFAULT_MAX_FILE_SIZE);

	if (data_buffer == NULL) {

		HandleError(MALLOC_ERROR, ERROR_IRRECOVERABLE);

	}
	
	file_size = GetFileSize();

	if (file_size == 0) {

		return FILE_IS_EMPTY;

	}

	if (file_size == ERROR_FILE_NOT_FOUND) {

		OpenFile(WRITE);
		CloseFile();
		return FILE_IS_EMPTY;

	}

	if (file_size > DEFAULT_MAX_FILE_SIZE) {

		if (file_size < EXTENDED_MAX_FILE_SIZE) {

			free(data_buffer);
			data_buffer = (char *)malloc(file_size);

			if (data_buffer == NULL) {

				HandleError(MALLOC_ERROR, ERROR_IRRECOVERABLE);

			}

		}

		else {

			HandleError(EXCEEDED_MAX_FILE_SIZE, ERROR_IRRECOVERABLE);

		}

	}

	OpenFile(READ);

	clearerr(data_file);
	fread(data_buffer, file_size, 1, data_file); // considering adding something to handle an fread error as well
	success = ferror(data_file);

	if (!success) {

		HandleError(FILE_READ_ERROR, ERROR_IRRECOVERABLE);

	}

	CloseFile();

	return 0;

}