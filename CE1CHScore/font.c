/* Implements HZK16 glyph reads and a fixed 64-entry LRU cache. */
/* Depends on CE 1.0 synchronous file APIs and the embedded ASCII glyph table. */
#include "font.h"
#include "ascii8x16.h"

#define CE1_HZK16_FILE_SIZE 267616UL

static void Ce1FontClearCache(CE1FONT *pFont)
{
    DWORD i;
    DWORD j;

    for (i = 0; i < CE1_FONT_CACHE_COUNT; ++i)
    {
        pFont->cache[i].code = 0;
        pFont->cache[i].age = 0;
        for (j = 0; j < CE1_FONT_GLYPH_BYTES; ++j)
        {
            pFont->cache[i].bits[j] = 0;
        }
    }
}

static void Ce1FontTouch(CE1FONT *pFont, DWORD recentIndex)
{
    DWORD i;

    for (i = 0; i < CE1_FONT_CACHE_COUNT; ++i)
    {
        if (i != recentIndex && pFont->cache[i].age != 0)
        {
            pFont->cache[i].age += 1;
        }
    }
    pFont->cache[recentIndex].age = 1;
}

static BOOL Ce1FontOpenFile(CE1FONT *pFont)
{
    DWORD fileSize;
    DWORD savedError;

    pFont->hFile = CreateFile(pFont->fontPath, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (pFont->hFile == INVALID_HANDLE_VALUE)
    {
        pFont->hFile = NULL;
        return FALSE;
    }

    pFont->ready = FALSE;
    fileSize = GetFileSize(pFont->hFile, NULL);
    if (fileSize == 0xFFFFFFFFUL)
    {
        savedError = GetLastError();
        if (savedError == ERROR_SUCCESS)
        {
            savedError = ERROR_INVALID_DATA;
        }
        if (!Ce1FontClose(pFont))
        {
            return FALSE;
        }
        SetLastError(savedError);
        return FALSE;
    }

    if (fileSize != CE1_HZK16_FILE_SIZE)
    {
        if (!Ce1FontClose(pFont))
        {
            return FALSE;
        }
        SetLastError(ERROR_INVALID_DATA);
        return FALSE;
    }

    Ce1FontClearCache(pFont);
    pFont->ready = TRUE;
    return TRUE;
}

BOOL Ce1FontOpen(CE1FONT *pFont, LPCTSTR pszFontPath)
{
    int pathLength;

    if (pFont == NULL || pszFontPath == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (pFont->hFile != NULL || pFont->ready)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    pathLength = lstrlen(pszFontPath);
    if (pathLength <= 0 || pathLength >= MAX_PATH)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    lstrcpy(pFont->fontPath, pszFontPath);
    pFont->ready = FALSE;
    Ce1FontClearCache(pFont);
    return Ce1FontOpenFile(pFont);
}

BOOL Ce1FontClose(CE1FONT *pFont)
{
    if (pFont == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (pFont->hFile == NULL)
    {
        pFont->ready = FALSE;
        Ce1FontClearCache(pFont);
        return TRUE;
    }

    if (!CloseHandle(pFont->hFile))
    {
        return FALSE;
    }

    pFont->hFile = NULL;
    pFont->ready = FALSE;
    Ce1FontClearCache(pFont);
    return TRUE;
}

BOOL Ce1FontGetGlyph(CE1FONT *pFont, DWORD code, BYTE *pBits)
{
    BYTE first;
    BYTE second;
    BYTE bits[CE1_FONT_GLYPH_BYTES];
    DWORD offset;
    DWORD bytesRead;
    DWORD i;
    DWORD j;
    DWORD victim;

    if (pFont == NULL || pBits == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (!pFont->ready || pFont->hFile == NULL)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (code >= 0x20 && code <= 0x7E)
    {
        for (i = 0; i < CE1_FONT_GLYPH_ROWS; ++i)
        {
            pBits[i * 2] = Ce1Ascii8x16[code * CE1_FONT_GLYPH_ROWS + i];
            pBits[i * 2 + 1] = 0;
        }
        return TRUE;
    }

    first = (BYTE)((code >> 8) & 0xFF);
    second = (BYTE)(code & 0xFF);
    if (code > 0xFFFF || first < 0xA1 || first > 0xF7 ||
        second < 0xA1 || second > 0xFE)
    {
        SetLastError(ERROR_INVALID_DATA);
        return FALSE;
    }

    for (i = 0; i < CE1_FONT_CACHE_COUNT; ++i)
    {
        if (pFont->cache[i].age != 0 && pFont->cache[i].code == code)
        {
            Ce1FontTouch(pFont, i);
            for (j = 0; j < CE1_FONT_GLYPH_BYTES; ++j)
            {
                pBits[j] = pFont->cache[i].bits[j];
            }
            return TRUE;
        }
    }

    offset = (((DWORD)(first - 0xA1) * 94UL) +
              (DWORD)(second - 0xA1)) * CE1_FONT_GLYPH_BYTES;
    if (SetFilePointer(pFont->hFile, (LONG)offset, NULL, FILE_BEGIN) ==
        0xFFFFFFFFUL)
    {
        return FALSE;
    }

    bytesRead = 0;
    if (!ReadFile(pFont->hFile, bits, CE1_FONT_GLYPH_BYTES, &bytesRead, NULL))
    {
        return FALSE;
    }
    if (bytesRead != CE1_FONT_GLYPH_BYTES)
    {
        SetLastError(ERROR_READ_FAULT);
        return FALSE;
    }

    victim = 0;
    for (i = 0; i < CE1_FONT_CACHE_COUNT; ++i)
    {
        if (pFont->cache[i].age == 0)
        {
            victim = i;
            break;
        }
        if (pFont->cache[i].age > pFont->cache[victim].age)
        {
            victim = i;
        }
    }

    pFont->cache[victim].code = code;
    for (j = 0; j < CE1_FONT_GLYPH_BYTES; ++j)
    {
        pFont->cache[victim].bits[j] = bits[j];
        pBits[j] = bits[j];
    }
    Ce1FontTouch(pFont, victim);
    return TRUE;
}

BOOL Ce1FontHibernate(CE1FONT *pFont)
{
    return Ce1FontClose(pFont);
}

BOOL Ce1FontActivate(CE1FONT *pFont)
{
    if (pFont == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (pFont->ready)
    {
        if (pFont->hFile != NULL)
        {
            return TRUE;
        }
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (pFont->hFile != NULL && !Ce1FontClose(pFont))
    {
        return FALSE;
    }

    if (pFont->fontPath[0] == 0)
    {
        SetLastError(ERROR_INVALID_DATA);
        return FALSE;
    }

    return Ce1FontOpenFile(pFont);
}
