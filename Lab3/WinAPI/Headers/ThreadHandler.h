#ifndef LAB3_WINAPI_THREADHANDLER_H
#define LAB3_WINAPI_THREADHANDLER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

DWORD WINAPI threadWorker(LPVOID threadParam);

#endif
