/* Defines streamed page layout for CE1CHS. */
/* Depends on the font, DBCS, and monochrome line-rendering modules. */
#ifndef CE1_LAYOUT_H
#define CE1_LAYOUT_H

#include "font.h"

/* A null DC measures without drawing. The next offset receives the next page. */
BOOL Ce1LayoutPage(CE1FONT *pFont, HDC hDC, const BYTE *pBuffer,
                   DWORD bufLen, DWORD startOffset, const RECT *pRect,
                   DWORD *pNextOffset);

#endif
