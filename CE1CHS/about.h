/* Defines the lightweight about page using the native application icon. */
#ifndef CE1_ABOUT_H
#define CE1_ABOUT_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>
#include "ce1core.h"

BOOL Ce1AboutPaint(HWND hWnd, CE1COREHANDLE hCore, HICON hIcon,
                   HBITMAP hLogo1, HBITMAP hLogo2, int commandBarHeight);

#endif
