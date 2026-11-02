/* Implements streamed mixed ASCII and GB2312 page layout. */
/* Depends on bounded DBCS decoding, font glyphs, and one-line GDI rendering. */
#include "layout.h"
#include "dbcs.h"
#include "render.h"

#define CE1_LAYOUT_TAB_WIDTH 32UL

static DWORD Ce1LayoutError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

static void Ce1LayoutClearLine(BYTE *pLine, DWORD bufferSize)
{
    DWORD i;

    for (i = 0; i < bufferSize; ++i)
    {
        pLine[i] = 0xFF;
    }
}

static void Ce1LayoutPlaceGlyph(BYTE *pLine, DWORD stride, DWORD x,
                                DWORD width, const BYTE *pGlyph)
{
    DWORD row;
    DWORD column;
    DWORD destination;
    BYTE mask;

    for (row = 0; row < CE1_RENDER_LINE_HEIGHT; ++row)
    {
        for (column = 0; column < width; ++column)
        {
            mask = (BYTE)(0x80 >> (column & 7));
            if ((pGlyph[row * 2 + (column >> 3)] & mask) != 0)
            {
                destination = x + column;
                mask = (BYTE)(0x80 >> (destination & 7));
                pLine[row * stride + (destination >> 3)] &= (BYTE)(~mask);
            }
        }
    }
}

BOOL Ce1LayoutPage(CE1FONT *pFont, HDC hDC, const BYTE *pBuffer,
                   DWORD bufLen, DWORD startOffset, const RECT *pRect,
                   DWORD *pNextOffset)
{
    CE1GLYPH glyph;
    BYTE glyphBits[CE1_FONT_GLYPH_BYTES];
    BYTE *pLine;
    HANDLE hHeap;
    DWORD width;
    DWORD height;
    DWORD stride;
    DWORD bufferSize;
    DWORD offset;
    DWORD lineY;
    DWORD x;
    DWORD drawWidth;
    DWORD glyphCode;
    DWORD savedError;
    BOOL hasGlyph;
    BOOL success;

    if (pNextOffset == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    *pNextOffset = startOffset;

    if (pFont == NULL || pRect == NULL || startOffset > bufLen ||
        (bufLen != 0 && pBuffer == NULL))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (pRect->left < 0 || pRect->top < 0 ||
        pRect->right <= pRect->left || pRect->bottom <= pRect->top)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    width = (DWORD)(pRect->right - pRect->left);
    height = (DWORD)(pRect->bottom - pRect->top);
    if (height < CE1_RENDER_LINE_HEIGHT || width > 0xFFFFFFF0UL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    stride = ((width + 15UL) / 16UL) * 2UL;
    if (stride == 0 || stride > 0xFFFFFFFFUL / CE1_RENDER_LINE_HEIGHT)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    bufferSize = stride * CE1_RENDER_LINE_HEIGHT;

    pLine = NULL;
    hHeap = NULL;
    offset = startOffset;
    lineY = 0;
    savedError = ERROR_SUCCESS;
    success = FALSE;

    if (hDC != NULL && offset < bufLen)
    {
        hHeap = GetProcessHeap();
        if (hHeap == NULL)
        {
            savedError = Ce1LayoutError();
            goto cleanup;
        }

        pLine = (BYTE *)HeapAlloc(hHeap, 0, bufferSize);
        if (pLine == NULL)
        {
            savedError = ERROR_NOT_ENOUGH_MEMORY;
            goto cleanup;
        }
    }

    while (lineY + CE1_RENDER_LINE_HEIGHT <= height && offset < bufLen)
    {
        x = 0;
        if (pLine != NULL)
        {
            Ce1LayoutClearLine(pLine, bufferSize);
        }

        while (offset < bufLen)
        {
            if (!Ce1NextGlyph(pBuffer, bufLen, offset, &glyph))
            {
                savedError = Ce1LayoutError();
                goto cleanup;
            }

            if (glyph.kind == CE1_GLYPH_EOF)
            {
                offset = bufLen;
                break;
            }

            if (glyph.kind == CE1_GLYPH_NEWLINE)
            {
                offset = glyph.nextOffset;
                break;
            }

            hasGlyph = FALSE;
            glyphCode = glyph.code;
            if (glyph.kind == CE1_GLYPH_ASCII ||
                glyph.kind == CE1_GLYPH_CJK)
            {
                drawWidth = glyph.width;
                hasGlyph = TRUE;
            }
            else if (glyph.kind == CE1_GLYPH_INVALID)
            {
                drawWidth = 8;
                glyphCode = 0x3F;
                hasGlyph = TRUE;
            }
            else if (glyph.kind == CE1_GLYPH_CONTROL)
            {
                drawWidth = 8;
            }
            else if (glyph.kind == CE1_GLYPH_TAB)
            {
                drawWidth = CE1_LAYOUT_TAB_WIDTH - (x % CE1_LAYOUT_TAB_WIDTH);
            }
            else
            {
                savedError = ERROR_INVALID_DATA;
                goto cleanup;
            }

            if (drawWidth > width - x)
            {
                if (x != 0)
                {
                    break;
                }
                drawWidth = width;
            }

            if (pLine != NULL && hasGlyph)
            {
                if (!Ce1FontGetGlyph(pFont, glyphCode, glyphBits))
                {
                    savedError = Ce1LayoutError();
                    goto cleanup;
                }
                Ce1LayoutPlaceGlyph(pLine, stride, x, drawWidth, glyphBits);
            }

            x += drawWidth;
            offset = glyph.nextOffset;
        }

        if (pLine != NULL)
        {
            if (!Ce1RenderLine(hDC, pRect->left, pRect->top + (int)lineY,
                               (int)width, pLine))
            {
                savedError = Ce1LayoutError();
                goto cleanup;
            }
        }
        lineY += CE1_RENDER_LINE_HEIGHT;
    }

    success = TRUE;

cleanup:
    if (pLine != NULL)
    {
        if (!HeapFree(hHeap, 0, (LPVOID)pLine) && success)
        {
            success = FALSE;
            savedError = Ce1LayoutError();
        }
    }

    if (success)
    {
        *pNextOffset = offset;
    }
    else
    {
        if (savedError == ERROR_SUCCESS)
        {
            savedError = ERROR_GEN_FAILURE;
        }
        SetLastError(savedError);
    }
    return success;
}
