/* Implements one-line 1bpp bitmap drawing for CE1CHS. */
/* Depends only on documented CE 1.0 GDI creation, transfer, and cleanup APIs. */
#include "render.h"

static DWORD Ce1RenderError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

BOOL Ce1RenderLine(HDC hDC, int x, int y, int width, const BYTE *pBits)
{
    HDC hMemoryDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;
    COLORREF oldTextColor;
    COLORREF oldBkColor;
    DWORD savedError;
    BOOL success;

    if (hDC == NULL || pBits == NULL || width <= 0)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    hMemoryDC = NULL;
    hBitmap = NULL;
    hOldBitmap = NULL;
    oldTextColor = CLR_INVALID;
    oldBkColor = CLR_INVALID;
    savedError = ERROR_SUCCESS;
    success = FALSE;

    oldTextColor = SetTextColor(hDC, RGB(0, 0, 0));
    if (oldTextColor == CLR_INVALID)
    {
        savedError = Ce1RenderError();
        goto cleanup;
    }

    oldBkColor = SetBkColor(hDC, RGB(255, 255, 255));
    if (oldBkColor == CLR_INVALID)
    {
        savedError = Ce1RenderError();
        goto cleanup;
    }

    hBitmap = CreateBitmap(width, CE1_RENDER_LINE_HEIGHT, 1, 1, pBits);
    if (hBitmap == NULL)
    {
        savedError = Ce1RenderError();
        goto cleanup;
    }

    hMemoryDC = CreateCompatibleDC(hDC);
    if (hMemoryDC == NULL)
    {
        savedError = Ce1RenderError();
        goto cleanup;
    }

    hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, (HGDIOBJ)hBitmap);
    if (hOldBitmap == NULL)
    {
        savedError = Ce1RenderError();
        goto cleanup;
    }

    if (!BitBlt(hDC, x, y, width, CE1_RENDER_LINE_HEIGHT,
                hMemoryDC, 0, 0, SRCCOPY))
    {
        savedError = Ce1RenderError();
        goto cleanup;
    }

    success = TRUE;

cleanup:
    if (hOldBitmap != NULL)
    {
        if (SelectObject(hMemoryDC, (HGDIOBJ)hOldBitmap) == NULL && success)
        {
            success = FALSE;
            savedError = Ce1RenderError();
        }
    }

    if (hBitmap != NULL)
    {
        if (!DeleteObject((HGDIOBJ)hBitmap) && success)
        {
            success = FALSE;
            savedError = Ce1RenderError();
        }
    }

    if (hMemoryDC != NULL)
    {
        if (!DeleteDC(hMemoryDC) && success)
        {
            success = FALSE;
            savedError = Ce1RenderError();
        }
    }

    if (oldBkColor != CLR_INVALID)
    {
        if (SetBkColor(hDC, oldBkColor) == CLR_INVALID && success)
        {
            success = FALSE;
            savedError = Ce1RenderError();
        }
    }

    if (oldTextColor != CLR_INVALID)
    {
        if (SetTextColor(hDC, oldTextColor) == CLR_INVALID && success)
        {
            success = FALSE;
            savedError = Ce1RenderError();
        }
    }

    if (!success)
    {
        if (savedError == ERROR_SUCCESS)
        {
            savedError = ERROR_GEN_FAILURE;
        }
        SetLastError(savedError);
    }
    return success;
}
