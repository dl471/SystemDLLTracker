# SystemDLLTracker
## About
SystemDLLTracker is a plugin for Ollydbg 1.10. It keeps track of the issystemdll data for known modules using an external file. This is intended to give said data some degree of persistence so that it is not lost and will not require manual resetting between instances of Ollydbg.

## Usage
Place the SystemDLLTracker DLL into a compatible Ollydbg folder and Ollydbg will load it automatically.

Due to the lack of suitable plugin callbacks in Ollydbg 1.10 compared to Ollydbg 2.0 and performance concerns regarding an automated solution the issystemdll data must be told to update manually. This can be done by clicking the "Save System DLL data to file" button that can be found in the modules window or under the plugins drop down menu.

## Compilation
Can be compiled by opening SystemDLLTracker.sln with any compatible version of Visual Studio and building the solution.

Can also be compiled from the command line.

Microsoft (R) C/C++ Optimizing Compiler
`cl.exe /LD /J main.cpp file_handler.cpp module_list.cpp Ollydbg.lib /link /out:SystemDLLTracker.dll`