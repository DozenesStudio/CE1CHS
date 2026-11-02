/* Defines the lightweight direct-drawn document reader. */
#ifndef CE1_READER_H
#define CE1_READER_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>
#include "ce1core.h"
#include "document.h"

BOOL Ce1ReaderContentRect(HWND hWnd, int commandBarHeight, RECT *pRect);
DWORD Ce1ReaderAlignOffset(const CE1DOCUMENT *pDocument, DWORD offset);
BOOL Ce1ReaderPaint(HWND hWnd, CE1COREHANDLE hCore,
                    const CE1DOCUMENT *pDocument, DWORD pageStart,
                    BOOL hibernated, int commandBarHeight);

#endif
