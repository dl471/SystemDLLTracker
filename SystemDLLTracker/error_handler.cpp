#ifndef _ERROR_HANDLER
#define _ERROR_HANDLER
#include "error_handler.h"
#endif

void HandleError(int error, int severity) {

	// my plan was to have all error messages come here in order to ensure they were centralized and thus non-repeating in source and easy to change if necessary, but the likelihood of certain error messages requiring custom messages makes me think this may have been mistaken compared to the original idea of constructing error messages locally and sending them here later.

	char error_message[TEXTLEN];

	switch (error) {
	case EXCEEDED_MAX_FILE_SIZE: // actual logging of error information pending
		break;
	case MALLOC_ERROR:
		break;
	case FILE_OPEN_FAILURE:
		break;
	case FILE_CLOSE_FAILURE:
		break;
	case UNDEFINED_FILE_ERROR:
		break;
	case GFSE_ERROR:
		break;
	case CLOSE_HANDLE_ERROR:
		break;
	case FILE_READ_ERROR:
		break;
	case LOST_MODULE_LIST:
		break;
	default:
		break;
	}

	if (severity == ERROR_IRRECOVERABLE) {

		Flash("%s: Irrecoverable error, plugin halted", project_name);
		Addtolist(0, 0, "%s: Irrecoverable error, plugin halted", project_name); // will most likely have a more pronounced declaration of error than this
		plugin_halted = 1;

	}

}