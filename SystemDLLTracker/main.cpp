#ifndef _MAIN_HEADER
#include "main_header.h"
#define _MAIN_HEADER
#endif

#ifndef _MODULE_LIST
#define _MODULE_LIST
#include "module_list.h"
#endif

// extc and _export are defined in plugin.h


int retcode;
int previous_modules = 0;
t_table *modules;
t_module *temp_module_pointer;
int *number_of_modules;
int nom;

int failure = 0;
int file_failure = 0;
int memory_failure = 0;
int catastrophic_failure = 0; // these aren't used yet, but will in the future probably be used to at least prevent a full odbg crash if there's a problem with the plugin

int save_data_displayed_in_module_window = 1;

const char *project_name = "SystemDLLTracker";
const char *project_save_file = "system_dll_tracker.txt";

ModuleList *module_list;

void LoadUserPreferences(int *save_data_displayed_in_module_window) {
	// these functions will most likely be completely depreceated by ollydbg's own .ini storage or made to work with it
	*save_data_displayed_in_module_window = 1;

}

void SaveUserPreferences(int *save_data_displayed_in_module_window) {



}




extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
	
	strcpy_s(shortname, 32, project_name);
	return PLUGIN_VERSION;

}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw, ulong *features) {

	if (ollydbgversion > 110) {

		return -1; // consider writing to log window or annoying pop up here - either way, there needs to be some sort of sign that things failed to load

	}
	else { // i am aware that the else here is 100% unnecessary, but i think it's prettier this way

		module_list = NULL;
		ODBG_Pluginreset();
		return 0; 

	}

}

extc void _export cdecl ODBG_Pluginreset(void) {

	modules = (t_table *)Plugingetvalue(VAL_MODULES);
	number_of_modules = &(modules->data.n);
	previous_modules = 0;

	LoadUserPreferences(&save_data_displayed_in_module_window);
	
	if (module_list != NULL) {

		delete module_list;

	}

	module_list = new ModuleList();

}

extc int _export cdecl ODBG_Pluginclose(void) {
	
	//Pluginwriteinttoini() // need DLL_MAIN etc. for this to work, focusing on other stuff for now

	return 0; 

}

extc void _export cdecl ODBG_Plugindestroy(void) {

	delete module_list;

}

int CheckData() {

	int i = 0;

	for (i = 0; i < *number_of_modules; i++) {

		temp_module_pointer = (t_module*)Getsortedbyselection(&modules->data, i);

		switch (module_list->PresentInList(temp_module_pointer->path)) {

		case NOT_PRESENT:
			module_list->AddNode(temp_module_pointer, FROM_LIVE_DATA);
			break;

		case PRESENT_UP_TO_DATE:
			module_list->UpdateIsSystemDLLInfo(temp_module_pointer->path, temp_module_pointer->issystemdll); 
			break;

		case PRESENT_MARK_SYSTEM:
			temp_module_pointer->issystemdll = 1;
			module_list->MarkUpdated(); // originally this was done in the PresentInList function, but was moved here to facilitate future error checking if needed. 
			break;

		case PRESENT_MARK_NON_SYSTEM:
			temp_module_pointer->issystemdll = 0;
			module_list->MarkUpdated();
			break;

		}

	}

	return 0;

}

extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) {

	// originally my plan was to use pluginnotify to only work when new modules were loaded but this turned out to be >2.0 callback that is unavailable in 1.10, so i was forced to check on main loop since i considered the possiblity of missing a moudle to be unacceptable

	nom = *number_of_modules; // getting pointer once and reusing to avoid calling getplugindata constantly, but need to be very careful with the idea said pointer will be freed if the module table needs to be reallocated. the idea that the table itself will be reallocated as opposed to merely one of its members is considered very very unlikely, but the possiblity posses a severe risk.

	if (nom != previous_modules) { // not using > to cover cases of unloading one module then loading another

		CheckData();
		previous_modules = nom;
	
	}

}

extc int  _export cdecl ODBG_Pluginmenu(int origin, char data[4096], void *item) {

	switch (origin) {

		case PM_MAIN:
			strcpy_s(data, 100, "0 &Save SystemDLL data to file|1 &About");
			return 1;

		case PM_MODULES:
			strcpy_s(data, 100, "0 &Save SystemDLL data to file");
			return 1;

	}

	return 0;

}

extc void _export cdecl ODBG_Pluginaction(int origin, int action, void *item) {

	switch (origin) {

		case PM_MAIN:
			if (action == 0) {
				CheckData();
				Flash("System DLL data updated!"); // is this an appropriate use of flash, or is flash for errors? 
			}
			if (action == 1) {
				// show about window, i don't have an about window, i will be hitting the win32 api to make an about window
			}
			break;

		case PM_MODULES:
			if (action == 0) {
				CheckData();
				Flash("System DLL data updated!");
			}
			break;

	}

}

