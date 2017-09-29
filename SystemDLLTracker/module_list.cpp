#ifndef _MODULE_LIST
#define _MODULE_LIST
#include "module_list.h"
#endif

ModuleList::ModuleList() {

	number_of_nodes = 0;

	first = NULL;
	last = NULL;

	file_io = new FileHandler();

	LoadFile();
	
}

ModuleList::~ModuleList() {

	SaveFile();
	FreeNodes();

	delete file_io;

}

int ModuleList::AddNode(t_module *target, int last_update_from_file) {

	node *new_node;
	new_node = new node;

	ZeroStrings(new_node);

	memmove(new_node->short_name, target->name, SHORTLEN);
	memmove(new_node->full_name, target->path, MAX_PATH);

	if (first == NULL) {

		first = new_node;
		last = NULL; // superflous due to constructor already initalizing last to NULL and last being updated a few lines later, strongly considering removing

	}

	new_node->is_system_dll = target->issystemdll;
	new_node->last_update_from_file = last_update_from_file;

	new_node->prev = last;
	last = new_node;

	number_of_nodes += 1;

	return 0;

}

int ModuleList::ZeroStrings(node *new_node) {

	memset(new_node->short_name, 0x00, SHORTLEN+1);
	memset(new_node->full_name, 0x00, MAX_PATH+1);

	return 0;

}

node *ModuleList::SearchNodes(char *path) {

	node *temp_node;

	if (number_of_nodes == 0) {

		return NULL;

	}

	if (first == NULL || last == NULL) {

		// reporting existing nodes but no list could be found - also this check and soon its body has found itself being repeated three times, could it be a candidate to become a seperate function?
		// cannot return, this check will call the new error handler directly once it is written

	}

	temp_node = last;
	
	while (1) { // consider changing to be based on number of nodes instead of checking null

		if (strncmp(path, temp_node->full_name, MAX_PATH) == 0) { 
				
			return temp_node; 
			
		}
			
		temp_node = temp_node->prev;

		if (temp_node == NULL) { 
				
			return NULL;
			
		}

	}

}

int ModuleList::PresentInList(char *path) {

	node *temp_node;

	temp_node = SearchNodes(path);

	if (temp_node == NULL) {

		return NOT_PRESENT;

	}

	if (temp_node->last_update_from_file && temp_node->is_system_dll) {

		temp_node->last_update_from_file = 0;
		last_accessed = temp_node;
		return PRESENT_MARK_SYSTEM;

	}

	if (temp_node->last_update_from_file && !(temp_node->is_system_dll)) {

		temp_node->last_update_from_file = 0;
		last_accessed = temp_node;
		return PRESENT_MARK_NON_SYSTEM;

	}

	else {

		last_accessed = temp_node;
		return PRESENT_UP_TO_DATE;

	}
	
}

int ModuleList::MarkUpdated() {

	last_accessed->last_update_from_file = 0;

	return 0;

}

int ModuleList::UpdateIsSystemDLLInfo(char *path, int issystemdll) {

	last_accessed->is_system_dll = issystemdll;

	return 0;

}

int ModuleList::FreeNodes() {

	
	if (number_of_nodes > 0) {

		node *temp_node = last;
		node *temp_node_prev;


		if (first == NULL || last == NULL) {

			return -1; // will call new error handler once it is written

		}

		while (1) { 

			if (temp_node == NULL) {

				break;

			}

			temp_node_prev = temp_node->prev;
			delete temp_node;
			temp_node = temp_node_prev;

		}

	}

	return 0;

}

int ModuleList::LoadFile() {

	t_module *temp;
	int ret;
	
	ret = file_io->ReadFile();

	if (ret == FILE_IS_EMPTY) {
		
		return 0; 
	
	}
	
	while (1) {

		temp = file_io->FirstRoundValidate();

		if (temp == NULL) {
		
			break; 
		
		}
		else {

			AddNode(temp, FROM_FILE_RECORD);
			delete temp;

		}
		
	}
	
	return 0;

}

int ModuleList::SaveFile() {

	node *temp_node;

	if (number_of_nodes == 0) {

		return 0;

	}

	if (first == NULL || last == NULL) {

		return -1;  // will call new error handler once it is written

	}

	temp_node = last;

	file_io->OpenFile(WRITE);

	//for (i = 0; i < number_of_nodes; i++) {
	while (1) {

		if (temp_node == NULL) {

			break;

		}

		file_io->CommitToFile(temp_node->short_name, temp_node->full_name, temp_node->is_system_dll);

		temp_node = temp_node->prev;

	}

	file_io->CloseFile(); // i wanted opening and closing the file to be part of FileHandler (i.e. a part of CommitToFile) but making sure there were no overrwites and deciding when to close the file without and explicit "done writing" signal seemed like it wouldn't be worth it

	return 0;


}