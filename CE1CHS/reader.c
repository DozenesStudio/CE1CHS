/* Implements direct reader painting without backing bitmaps or controls. */
#include "reader.h"
#include "ui.h"

#define CE1_READER_LINE_HEIGHT 16

static DWORD Ce1ReaderError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

BOOL Ce1ReaderContentRect(HWND hWnd, int commandBarHeight, RECT *pRect)
{
    return Ce1UiContentRect(hWnd, commandBarHeight, pRect);
}

DWORD Ce1ReaderAlignOffset(const CE1DOCUMENT *pDocument, DWORD offset)
{
    DWORD start;

    if (offset == 0 || offset >= pDocument->length)
    {
        return offset;
    }

    start = offset;
    while (start > 0 && pDocument->pBytes[start - 1] >= 0x80)
    {
        --start;
    }
    if (((offset - start) & 1) != 0)
    {
        --offset;
    }
    return offset;
}

BOOL Ce1ReaderPaint(HWND hWnd, CE1COREHANDLE hCore,
                    const CE1DOCUMENT *pDocument, DWORD pageStart,
                    BOOL hibernated, int commandBarHeight)
{
    PAINTSTRUCT paint;
    RECT content;
    HDC hDC;
    HBRUSH hBrush;
    DWORD nextOffset;
    DWORD savedError;
    BOOL success;

    hDC = BeginPaint(hWnd, &paint);
    if (hDC == NULL)
    {
        return FALSE;
    }

    savedError = ERROR_SUCCESS;
    success = FALSE;
    nextOffset = pageStart;
    if (!Ce1ReaderContentRect(hWnd, commandBarHeight, &content))
    {
        savedError = Ce1ReaderError();
        goto cleanup;
    }
    if (content.right <= content.left || content.bottom <= content.top)
    {
        success = TRUE;
        goto cleanup;
    }

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    if (hBrush == NULL || !FillRect(hDC, &content, hBrush))
    {
        savedError = Ce1ReaderError();
        goto cleanup;
    }

    if (pDocument != NULL && pDocument->loaded && pDocument->pBytes != NULL &&
        !hibernated && hCore != NULL && pageStart < pDocument->length &&
        content.bottom - content.top >= CE1_READER_LINE_HEIGHT)
    {
        if (!CE1CoreDrawPage(hCore, hDC, pDocument->pBytes,
                             pDocument->length, pageStart, &content,
                             &nextOffset))
        {
            savedError = Ce1ReaderError();
            goto cleanup;
        }
        if (nextOffset <= pageStart || nextOffset > pDocument->length)
        {
            savedError = ERROR_INVALID_DATA;
            goto cleanup;
        }
    }
    success = TRUE;

cleanup:
    EndPaint(hWnd, &paint);
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
