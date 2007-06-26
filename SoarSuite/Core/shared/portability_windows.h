#ifndef PORTABILITY_WINDOWS_H
#define PORTABILITY_WINDOWS_H

/* This file contains code specific to the windows platforms */

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0400		// This is required since our target is NT4+

#include <windows.h>
#include <process.h>
#include <winsock2.h>
#include <direct.h>
#include <assert.h>
#include <Lmcons.h> // for UNLEN constant
#include <malloc.h>
#include <conio.h>

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024   /* AGR 536  - from sys/param.h */
#endif

// Map certain functions depending on the OS
#define getcwd _getcwd
#define chdir _chdir
#define strcasecmp _stricmp
#define vsnprintf _vsnprintf
#define snprintf _snprintf 
#define ENABLE_NAMED_PIPES

// BADBAD: Not sure where this macro is coming from but I saw this
// in IgSKI_Symbol.h and it's needed for the GetObject() calls to compile.
#ifdef _WIN32
#undef GetObject
#undef SendMessage
#endif

#endif // PORTABILITY_WINDOWS_H