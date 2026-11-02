/* Implements stateless ASCII and GB2312 decoding for CE1CHS. */
/* Depends on dbcs.h and never uses NUL termination or CRT string functions. */
#include "dbcs.h"

BOOL Ce1NextGlyph(const BYTE *pBuffer, DWORD bufLen, DWORD offset,
                  CE1GLYPH *pGlyph)
{
    BYTE first;
    BYTE second;

    if (pGlyph == NULL || offset > bufLen ||
        (bufLen != 0 && pBuffer == NULL))
    {
        return FALSE;
    }

    pGlyph->kind = CE1_GLYPH_EOF;
    pGlyph->code = 0;
    pGlyph->width = 0;
    pGlyph->nextOffset = offset;

    if (offset == bufLen)
    {
        return TRUE;
    }

    first = pBuffer[offset];
    pGlyph->code = first;
    pGlyph->nextOffset = offset + 1;

    if (first == 0x09)
    {
        pGlyph->kind = CE1_GLYPH_TAB;
        return TRUE;
    }

    if (first == 0x0A)
    {
        pGlyph->kind = CE1_GLYPH_NEWLINE;
        return TRUE;
    }

    if (first == 0x0D)
    {
        if (offset + 1 < bufLen && pBuffer[offset + 1] == 0x0A)
        {
            pGlyph->kind = CE1_GLYPH_NEWLINE;
            pGlyph->code = 0x0A;
            pGlyph->nextOffset = offset + 2;
        }
        else
        {
            pGlyph->kind = CE1_GLYPH_CONTROL;
        }
        return TRUE;
    }

    if (first >= 0x20 && first <= 0x7E)
    {
        pGlyph->kind = CE1_GLYPH_ASCII;
        pGlyph->width = 8;
        return TRUE;
    }

    if (first < 0x20 || first == 0x7F)
    {
        pGlyph->kind = CE1_GLYPH_CONTROL;
        return TRUE;
    }

    if (first >= 0xA1 && first <= 0xF7)
    {
        if (offset + 1 < bufLen)
        {
            second = pBuffer[offset + 1];
            if (second >= 0xA1 && second <= 0xFE)
            {
                pGlyph->kind = CE1_GLYPH_CJK;
                pGlyph->code = ((DWORD)first << 8) | (DWORD)second;
                pGlyph->width = 16;
                pGlyph->nextOffset = offset + 2;
                return TRUE;
            }
        }

        pGlyph->kind = CE1_GLYPH_INVALID;
        pGlyph->width = 8;
        return TRUE;
    }

    pGlyph->kind = CE1_GLYPH_INVALID;
    pGlyph->width = 8;
    return TRUE;
}
