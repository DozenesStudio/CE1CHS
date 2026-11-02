/* Defines HZK16 access and the fixed glyph cache for CE1CHS. */
/* Depends on Windows CE base types and an absolute font file path. */
#ifndef CE1_FONT_H
#define CE1_FONT_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

#define CE1_FONT_CACHE_COUNT  64
#define CE1_FONT_GLYPH_BYTES  32
#define CE1_FONT_GLYPH_ROWS   16

typedef struct CE1FONTCACHEENTRY
{
    DWORD code;
    DWORD age;
    BYTE bits[CE1_FONT_GLYPH_BYTES];
} CE1FONTCACHEENTRY;

typedef struct CE1FONT
{
    HANDLE hFile;
    BOOL ready;
    TCHAR fontPath[MAX_PATH];
    CE1FONTCACHEENTRY cache[CE1_FONT_CACHE_COUNT];
} CE1FONT;

/* Opens and validates HZK16. The caller provides zeroed or closed storage. */
BOOL Ce1FontOpen(CE1FONT *pFont, LPCTSTR pszFontPath);

/* Closes the file and clears the cache. Repeated close calls are safe. */
BOOL Ce1FontClose(CE1FONT *pFont);

/* Returns a 16-row glyph. ASCII uses the left byte and clears the right byte. */
BOOL Ce1FontGetGlyph(CE1FONT *pFont, DWORD code, BYTE *pBits);

/* Releases the file and cache while retaining the path for activation. */
BOOL Ce1FontHibernate(CE1FONT *pFont);

/* Reopens and validates the retained path after hibernation. */
BOOL Ce1FontActivate(CE1FONT *pFont);

#endif
