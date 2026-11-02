/* Defines monochrome line rendering for CE1CHS. */
/* Depends on Windows CE 1.0 GDI and a WORD-aligned scan-line buffer. */
#ifndef CE1_RENDER_H
#define CE1_RENDER_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

#define CE1_RENDER_LINE_HEIGHT 16

/* Creates and draws one 1bpp line, then releases all temporary GDI objects. */
BOOL Ce1RenderLine(HDC hDC, int x, int y, int width, const BYTE *pBits);

#endif
