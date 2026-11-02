/* Implements dynamic home-page drawing and captured stylus interaction. */
/* Uses only CE GDI, bounded byte strings, and the existing core text layout. */
#include "home.h"
#include "ui.h"

#define CE1_HOME_BUTTON_COUNT  3
#define CE1_HOME_BUTTON_HEIGHT 28
#define CE1_HOME_BUTTON_GAP    8
#define CE1_HOME_RIGHT_MARGIN  8
#define CE1_HOME_TOP_MARGIN    8
#define CE1_HOME_TITLE_LINES   2
#define CE1_HOME_AUTHOR_GAP    4
#define CE1_HOME_RESERVED_TOP  68

static int g_homeAction = CE1_HOME_ACTION_NONE;
static BOOL g_homePressed;

static DWORD Ce1HomeError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

static DWORD Ce1HomeTextLength(const BYTE *pText)
{
    DWORD length;

    length = 0;
    while (pText[length] != 0)
    {
        ++length;
    }
    return length;
}

static DWORD Ce1HomeTextWidth(const BYTE *pText, DWORD start, DWORD end)
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

static BOOL Ce1HomeDrawText(CE1COREHANDLE hCore, HDC hDC, const BYTE *pText,
                            int left, int top, int width, int maxLines,
                            DWORD *pLines)
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

    length = Ce1HomeTextLength(pText);
    offset = 0;
    lines = 0;
    while (offset < length && lines < (DWORD)maxLines)
    {
        if (!CE1CoreMeasurePage(hCore, pText, length, offset, width,
                                CE1_HOME_BUTTON_HEIGHT - 12, &nextOffset))
        {
            return FALSE;
        }
        if (nextOffset <= offset || nextOffset > length)
        {
            SetLastError(ERROR_INVALID_DATA);
            return FALSE;
        }

        textWidth = Ce1HomeTextWidth(pText, offset, nextOffset);
        x = left;
        if (textWidth < (DWORD)width)
        {
            x += (width - (int)textWidth) / 2;
        }
        rect.left = x;
        rect.top = top + (int)lines * 16;
        rect.right = left + width;
        rect.bottom = rect.top + 16;
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

static BOOL Ce1HomeLayout(HWND hWnd, int commandBarHeight, RECT *pButtons)
{
    RECT client;
    CE1STRINGID ids[CE1_HOME_BUTTON_COUNT];
    DWORD textWidth;
    DWORD maxTextWidth;
    int width;
    int height;
    int buttonWidth;
    int totalHeight;
    int x;
    int y;
    int i;

    if (!Ce1UiContentRect(hWnd, commandBarHeight, &client))
    {
        return FALSE;
    }

    ids[0] = UI_STR_OPEN;
    ids[1] = UI_STR_SETTINGS;
    ids[2] = UI_STR_ABOUT;
    maxTextWidth = 0;
    for (i = 0; i < CE1_HOME_BUTTON_COUNT; ++i)
    {
        textWidth = Ce1HomeTextWidth(Ce1UiText(ids[i]), 0,
                                     Ce1HomeTextLength(Ce1UiText(ids[i])));
        if (textWidth > maxTextWidth)
        {
            maxTextWidth = textWidth;
        }
    }

    width = client.right - client.left;
    height = client.bottom - client.top;
    buttonWidth = (int)maxTextWidth + 16;
    if (buttonWidth < 72)
    {
        buttonWidth = 72;
    }
    if (width > 16 && buttonWidth > width - 16)
    {
        buttonWidth = width - 16;
    }
    else if (width <= 16)
    {
        buttonWidth = width;
    }

    x = width - CE1_HOME_RIGHT_MARGIN - buttonWidth;
    if (x < 0)
    {
        x = 0;
    }
    totalHeight = CE1_HOME_BUTTON_COUNT * CE1_HOME_BUTTON_HEIGHT +
                  (CE1_HOME_BUTTON_COUNT - 1) * CE1_HOME_BUTTON_GAP;
    if (height >= totalHeight)
    {
        y = (height - totalHeight) / 2;
        if (y < CE1_HOME_RESERVED_TOP)
        {
            y = CE1_HOME_RESERVED_TOP;
        }
        if (y + totalHeight > height)
        {
            y = height - totalHeight;
        }
    }
    else
    {
        y = 0;
    }

    for (i = 0; i < CE1_HOME_BUTTON_COUNT; ++i)
    {
        pButtons[i].left = client.left + x;
        pButtons[i].right = pButtons[i].left + buttonWidth;
        pButtons[i].top = client.top + y + i *
                          (CE1_HOME_BUTTON_HEIGHT + CE1_HOME_BUTTON_GAP);
        pButtons[i].bottom = pButtons[i].top + CE1_HOME_BUTTON_HEIGHT;
    }
    return TRUE;
}

static int Ce1HomeHitTest(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    RECT buttons[CE1_HOME_BUTTON_COUNT];
    POINT point;
    int i;

    if (!Ce1HomeLayout(hWnd, commandBarHeight, buttons))
    {
        return CE1_HOME_ACTION_NONE;
    }
    point.x = (int)(short)LOWORD(lParam);
    point.y = (int)(short)HIWORD(lParam);
    for (i = 0; i < CE1_HOME_BUTTON_COUNT; ++i)
    {
        if (PtInRect(&buttons[i], point))
        {
            return i;
        }
    }
    return CE1_HOME_ACTION_NONE;
}

static void Ce1HomeInvalidateButton(HWND hWnd, int commandBarHeight, int action)
{
    RECT buttons[CE1_HOME_BUTTON_COUNT];

    if (action >= 0 && action < CE1_HOME_BUTTON_COUNT &&
        Ce1HomeLayout(hWnd, commandBarHeight, buttons))
    {
        InvalidateRect(hWnd, &buttons[action], TRUE);
    }
}

void Ce1HomeInit(void)
{
    g_homeAction = CE1_HOME_ACTION_NONE;
    g_homePressed = FALSE;
}

void Ce1HomeCancel(HWND hWnd)
{
    int action;

    action = g_homeAction;
    g_homeAction = CE1_HOME_ACTION_NONE;
    g_homePressed = FALSE;
    if (action != CE1_HOME_ACTION_NONE)
    {
        ReleaseCapture();
    }
    (void)hWnd;
}

BOOL Ce1HomePaint(HWND hWnd, CE1COREHANDLE hCore, HBITMAP hBackground,
                  int commandBarHeight)
{
    PAINTSTRUCT paint;
    RECT client;
    RECT buttons[CE1_HOME_BUTTON_COUNT];
    RECT fill;
    HDC hDC;
    HBRUSH hBrush;
    CE1STRINGID ids[CE1_HOME_BUTTON_COUNT];
    DWORD titleLines;
    DWORD savedError;
    BOOL success;
    int i;

    hDC = BeginPaint(hWnd, &paint);
    if (hDC == NULL)
    {
        return FALSE;
    }

    titleLines = 0;
    savedError = ERROR_SUCCESS;
    success = FALSE;
    ids[0] = UI_STR_OPEN;
    ids[1] = UI_STR_SETTINGS;
    ids[2] = UI_STR_ABOUT;

    if (!Ce1UiContentRect(hWnd, commandBarHeight, &client))
    {
        savedError = Ce1HomeError();
        goto cleanup;
    }
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    if (hBrush == NULL)
    {
        savedError = Ce1HomeError();
        goto cleanup;
    }
    if (!FillRect(hDC, &client, hBrush))
    {
        savedError = Ce1HomeError();
        goto cleanup;
    }
    if (hBackground != NULL)
    {
        (void)Ce1UiDrawBitmap(hDC, hBackground, client.left, client.top);
    }
    if (client.right - client.left < 16 || client.bottom - client.top < 16)
    {
        success = TRUE;
        goto cleanup;
    }
    if (!Ce1HomeLayout(hWnd, commandBarHeight, buttons))
    {
        savedError = Ce1HomeError();
        goto cleanup;
    }

    if (!Ce1HomeDrawText(hCore, hDC, Ce1UiText(UI_STR_APP_TITLE),
                         client.left, client.top + CE1_HOME_TOP_MARGIN,
                         client.right - client.left, CE1_HOME_TITLE_LINES,
                         &titleLines))
    {
        savedError = Ce1HomeError();
        goto cleanup;
    }
    if (!Ce1HomeDrawText(hCore, hDC, Ce1UiText(UI_STR_AUTHOR), client.left,
                         client.top + CE1_HOME_TOP_MARGIN +
                         (int)titleLines * 16 + CE1_HOME_AUTHOR_GAP,
                         client.right - client.left, 1, NULL))
    {
        savedError = Ce1HomeError();
        goto cleanup;
    }

    for (i = 0; i < CE1_HOME_BUTTON_COUNT; ++i)
    {
        if (!Rectangle(hDC, buttons[i].left, buttons[i].top,
                       buttons[i].right, buttons[i].bottom))
        {
            savedError = Ce1HomeError();
            goto cleanup;
        }
        if (g_homeAction == i && g_homePressed)
        {
            hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
            if (hBrush == NULL)
            {
                savedError = Ce1HomeError();
                goto cleanup;
            }
            fill.left = buttons[i].left + 1;
            fill.top = buttons[i].top + 1;
            fill.right = buttons[i].right - 1;
            fill.bottom = buttons[i].bottom - 1;
            if (fill.right > fill.left && fill.bottom > fill.top &&
                !FillRect(hDC, &fill, hBrush))
            {
                savedError = Ce1HomeError();
                goto cleanup;
            }
        }
        if (!Ce1HomeDrawText(hCore, hDC, Ce1UiText(ids[i]),
                             buttons[i].left + 4, buttons[i].top + 6,
                             buttons[i].right - buttons[i].left - 8, 1, NULL))
        {
            savedError = Ce1HomeError();
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

void Ce1HomeMouseDown(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    int action;

    action = Ce1HomeHitTest(hWnd, commandBarHeight, lParam);
    if (action == CE1_HOME_ACTION_NONE)
    {
        return;
    }
    g_homeAction = action;
    g_homePressed = TRUE;
    SetCapture(hWnd);
    Ce1HomeInvalidateButton(hWnd, commandBarHeight, action);
}

void Ce1HomeMouseMove(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    BOOL pressed;

    if (g_homeAction == CE1_HOME_ACTION_NONE)
    {
        return;
    }
    pressed = (Ce1HomeHitTest(hWnd, commandBarHeight, lParam) == g_homeAction);
    if (pressed != g_homePressed)
    {
        g_homePressed = pressed;
        Ce1HomeInvalidateButton(hWnd, commandBarHeight, g_homeAction);
    }
}

int Ce1HomeMouseUp(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    int action;
    int hit;
    BOOL activate;

    if (g_homeAction == CE1_HOME_ACTION_NONE)
    {
        return CE1_HOME_ACTION_NONE;
    }
    action = g_homeAction;
    hit = Ce1HomeHitTest(hWnd, commandBarHeight, lParam);
    activate = (g_homePressed && hit == action);
    g_homeAction = CE1_HOME_ACTION_NONE;
    g_homePressed = FALSE;
    ReleaseCapture();
    Ce1HomeInvalidateButton(hWnd, commandBarHeight, action);
    return activate ? action : CE1_HOME_ACTION_NONE;
}
