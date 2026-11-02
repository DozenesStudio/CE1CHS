/* Defines bounded ASCII and GB2312 glyph decoding for CE1CHS. */
/* Depends only on Windows CE base types and explicit byte buffers. */
#ifndef CE1_DBCS_H
#define CE1_DBCS_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

typedef enum CE1GLYPHKIND
{
    CE1_GLYPH_EOF = 0,
    CE1_GLYPH_ASCII,
    CE1_GLYPH_CJK,
    CE1_GLYPH_TAB,
    CE1_GLYPH_NEWLINE,
    CE1_GLYPH_CONTROL,
    CE1_GLYPH_INVALID
} CE1GLYPHKIND;

typedef struct CE1GLYPH
{
    CE1GLYPHKIND kind;
    DWORD code;
    DWORD width;
    DWORD nextOffset;
} CE1GLYPH;

/* Decodes one glyph without reading beyond bufLen. */
/* Returns FALSE only for invalid arguments; end of input returns an EOF glyph. */
BOOL Ce1NextGlyph(const BYTE *pBuffer, DWORD bufLen, DWORD offset,
                  CE1GLYPH *pGlyph);

#endif
