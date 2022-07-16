//reference from https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#example-of-sgr-terminal-sequences

//BOTH anisc.c and anisc.h are the code from
//https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/
//they are not my code they are used for compability purpose only


#ifdef __APPLE__
#include <term.h>
#elif defined _WIN32 || defined _WIN64
#include <wchar.h>
#include <windows.h>
 #endif
 
 #include <stdio.h>
 #include "anisc.h"
 #ifdef __APPLE__
 void setupConsole(void) {}
 
 void restoreConsole(void) {
     // Reset colors
     printf("\x1b[0m");
 }
#elif defined _WIN32 || defined _WIN64
 // Some old MinGW/CYGWIN distributions don't define this:
 #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
 #define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
 #endif
 
 static HANDLE stdoutHandle;
 static DWORD outModeInit;
 
 void setupConsole(void) {
 	DWORD outMode = 0;
 	stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
 
 	if(stdoutHandle == INVALID_HANDLE_VALUE) {
 		exit(GetLastError());
 	}
 	
 	if(!GetConsoleMode(stdoutHandle, &outMode)) {
 		exit(GetLastError());
 	}
 
 	outModeInit = outMode;
 	
     // Enable ANSI escape codes
 	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
 
 	if(!SetConsoleMode(stdoutHandle, outMode)) {
 		exit(GetLastError());
 	}	
 }
 
 void restoreConsole(void) {
     // Reset colors
     printf("\x1b[0m");	
 	
     // Reset console mode
 	if(!SetConsoleMode(stdoutHandle, outModeInit)) {
 		exit(GetLastError());
 	}
 }
 #else
 void setupConsole(void) {}
 
 void restoreConsole(void) {
     // Reset colors
     printf("\x1b[0m");
 }
 #endif