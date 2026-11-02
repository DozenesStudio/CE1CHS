/* Implements about-page text and the shared native application icon. */
#include "about.h"
#include "ui.h"

#define CE1_ABOUT_MARGIN         12
#define CE1_ABOUT_NARROW_MARGIN  4
#define CE1_ABOUT_HEADER_GAP     12
#define CE1_ABOUT_LOGO_GAP       12
#define CE1_ABOUT_ICON_SIZE      32
#define CE1_ABOUT_TEXT_HEIGHT    16
#define CE1_ABOUT_TEXT_GAP         4
#define CE1_ABOUT_TEXT_COUNT       4
#define CE1_ABOUT_FILE_LOGO_SIZE   64
#define CE1_ABOUT_FILE_LOGO_GAP    10
#define CE1_ABOUT_FILE_LOGO_MARGIN 10

typedef struct CE1ABOUTLAYOUT
{
    RECT title;
    RECT logo;
    RECT text;
} CE1ABOUTLAYOUT;

static DWORD Ce1AboutError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

static DWORD Ce1AboutTextLength(const BYTE *pText)
{
    DWORD length;

    length = 0;
    while (pText[length] != 0)
    {
        ++length;
    }
    return length;
}

static DWORD Ce1AboutTextWidth(const BYTE *pText, DWORD start, DWORD end)
{
    DWORD offset;
    DWORD width;
    BYTE value;

    offset = start;
    width = 0;
    while (offset < end && pText[offset] != 0)
    {
        value = pText[offset];
        if (value == '\r' || value == '\n')
        {
            break;
        }
        if (value < 0x20 || value == 0x7F)
        {
            ++offset;
        }
        else if (value < 0x80)
        {
            width += 8;
            ++offset;
        }
        else
        {
            width += 16;
            offset += (offset + 1 < end && pText[offset + 1] != 0) ? 2 : 1;
        }
    }
    return width;
}

static BOOL Ce1AboutDrawText(CE1COREHANDLE hCore, HDC hDC, const BYTE *pText,
                             int left, int top, int width, int maxLines,
                             BOOL centered, DWORD *pLines)
{
    DWORD length;
    DWORD offset;
    DWORD nextOffset;
    DWORD drawOffset;
    DWORD textWidth;
    DWORD lines;
    RECT rect;
    int x;

    if (pLines != NULL)
    {
        *pLines = 0;
    }
    if (hCore == NULL || hDC == NULL || pText == NULL || width <= 0 ||
        maxLines <= 0)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    length = Ce1AboutTextLength(pText);
    offset = 0;
    lines = 0;
    while (offset < length && lines < (DWORD)maxLines)
    {
        if (!CE1CoreMeasurePage(hCore, pText, length, offset, width,
                                CE1_ABOUT_TEXT_HEIGHT, &nextOffset))
        {
            return FALSE;
        }
        if (nextOffset <= offset || nextOffset > length)
        {
            SetLastError(ERROR_INVALID_DATA);
            return FALSE;
        }

        x = left;
        if (centered)
        {
            textWidth = Ce1AboutTextWidth(pText, offset, nextOffset);
            if (textWidth < (DWORD)width)
            {
                x += (width - (int)textWidth) / 2;
            }
        }
        rect.left = x;
        rect.top = top + (int)lines * CE1_ABOUT_TEXT_HEIGHT;
        rect.right = left + width;
        rect.bottom = rect.top + CE1_ABOUT_TEXT_HEIGHT;
        if (!CE1CoreDrawPage(hCore, hDC, pText, length, offset, &rect,
                             &drawOffset))
        {
            return FALSE;
        }
        if (drawOffset != nextOffset)
        {
            SetLastError(ERROR_INVALID_DATA);
            return FALSE;
        }
        offset = nextOffset;
        ++lines;
    }

    if (pLines != NULL)
    {
        *pLines = lines;
    }
    return TRUE;
}

static BOOL Ce1AboutLayout(HWND hWnd, int commandBarHeight,
                           CE1ABOUTLAYOUT *pLayout)
{
    RECT client;
    int width;
    int margin;
    int left;
    int right;
    int contentTop;

    if (!Ce1UiContentRect(hWnd, commandBarHeight, &client))
    {
        return FALSE;
    }

    width = client.right - client.left;
    if (width >= 32)
    {
        margin = CE1_ABOUT_MARGIN;
    }
    else if (width >= 24)
    {
        margin = CE1_ABOUT_NARROW_MARGIN;
    }
    else
    {
        margin = 0;
    }
    left = client.left + margin;
    right = client.right - margin;

    pLayout->title.left = left;
    pLayout->title.right = right;
    pLayout->title.top = client.top + margin;
    pLayout->title.bottom = pLayout->title.top + CE1_ABOUT_TEXT_HEIGHT;

    contentTop = pLayout->title.bottom + CE1_ABOUT_HEADER_GAP;
    pLayout->logo.left = left;
    pLayout->logo.top = contentTop;
    pLayout->logo.right = left + CE1_ABOUT_ICON_SIZE;
    pLayout->logo.bottom = contentTop + CE1_ABOUT_ICON_SIZE;

    pLayout->text.left = pLayout->logo.right + CE1_ABOUT_LOGO_GAP;
    pLayout->text.top = contentTop;
    pLayout->text.right = right;
    pLayout->text.bottom = client.bottom;
    return TRUE;
}

BOOL Ce1AboutPaint(HWND hWnd, CE1COREHANDLE hCore, HICON hIcon,
                   HBITMAP hLogo1, HBITMAP hLogo2, int commandBarHeight)
{
    PAINTSTRUCT paint;
    RECT client;
    RECT window;
    CE1ABOUTLAYOUT layout;
    CE1STRINGID ids[CE1_ABOUT_TEXT_COUNT];
    HDC hDC;
    HBRUSH hBrush;
    DWORD savedError;
    DWORD lines;
    BOOL success;
    int textTop;
    int maxLines;
    int width;
    int height;
    int x1;
    int x2;
    int y;
    int i;

    hDC = BeginPaint(hWnd, &paint);
    if (hDC == NULL)
    {
        return FALSE;
    }

    savedError = ERROR_SUCCESS;
    success = FALSE;
    ids[0] = UI_STR_APP_TITLE;
    ids[1] = UI_STR_ABOUT_TAGLINE;
    ids[2] = UI_STR_ABOUT_AUTHOR;
    ids[3] = UI_STR_MADE_IN_CHINA;
    if (!Ce1UiContentRect(hWnd, commandBarHeight, &client))
    {
        savedError = Ce1AboutError();
        goto cleanup;
    }
    if (client.right <= client.left || client.bottom <= client.top)
    {
        success = TRUE;
        goto cleanup;
    }
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    if (hBrush == NULL || !FillRect(hDC, &client, hBrush))
    {
        savedError = Ce1AboutError();
        goto cleanup;
    }
    if (client.right - client.left < 16 || client.bottom - client.top < 16)
    {
        success = TRUE;
        goto cleanup;
    }
    if (!Ce1AboutLayout(hWnd, commandBarHeight, &layout))
    {
        savedError = Ce1AboutError();
        goto cleanup;
    }

    if (layout.title.right > layout.title.left &&
        !Ce1AboutDrawText(hCore, hDC, Ce1UiText(UI_STR_ABOUT),
                          layout.title.left, layout.title.top,
                          layout.title.right - layout.title.left, 1,
                          FALSE, NULL))
    {
        savedError = Ce1AboutError();
        goto cleanup;
    }
    if (hIcon == NULL)
    {
        savedError = ERROR_INVALID_HANDLE;
        goto cleanup;
    }
    if (!DrawIcon(hDC, layout.logo.left, layout.logo.top, hIcon))
    {
        savedError = Ce1AboutError();
        goto cleanup;
    }

    textTop = layout.text.top;
    if (layout.text.right > layout.text.left)
    {
        for (i = 0; i < CE1_ABOUT_TEXT_COUNT; ++i)
        {
            maxLines = (client.bottom - textTop) / CE1_ABOUT_TEXT_HEIGHT;
            if (maxLines <= 0)
            {
                break;
            }
            if (!Ce1AboutDrawText(hCore, hDC, Ce1UiText(ids[i]),
                                  layout.text.left, textTop,
                                  layout.text.right - layout.text.left,
                                  maxLines, FALSE, &lines))
            {
                savedError = Ce1AboutError();
                goto cleanup;
            }
            textTop += (int)lines * CE1_ABOUT_TEXT_HEIGHT;
            if (i + 1 < CE1_ABOUT_TEXT_COUNT)
            {
                textTop += CE1_ABOUT_TEXT_GAP;
            }
        }
    }

    if (hLogo1 != NULL || hLogo2 != NULL)
    {
        if (GetClientRect(hWnd, &window))
        {
            width = window.right - window.left;
            height = window.bottom - window.top;
            x1 = width - CE1_ABOUT_FILE_LOGO_SIZE * 2 -
                 CE1_ABOUT_FILE_LOGO_GAP * 2 - CE1_ABOUT_FILE_LOGO_MARGIN;
            x2 = x1 + CE1_ABOUT_FILE_LOGO_SIZE + CE1_ABOUT_FILE_LOGO_GAP;
            y = height - CE1_ABOUT_FILE_LOGO_SIZE -
                CE1_ABOUT_FILE_LOGO_MARGIN;
            if (x1 >= 0 && y >= commandBarHeight)
            {
                (void)Ce1UiDrawBitmap(hDC, hLogo1, window.left + x1,
                                      window.top + y);
                (void)Ce1UiDrawBitmap(hDC, hLogo2, window.left + x2,
                                      window.top + y);
            }
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
